#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/mman.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

static void server_job(int sd, int slot);

#define MINSPARSESERVERS 5
#define MAXSPARSESERVERS 10
#define MAXCLIENT 20
#define SERVERPORT 8888

#define LINESIZE 80
#define SIG_NOTIFY SIGUSR2

enum {
    STATE_IDLE = 0,
    STATE_BUSY
};

struct server_st {
    pid_t pid;
    int state;
};

typedef struct server_st msg_t;
static struct server_st *serverpool;
static int idle_count = 0;
static int busy_count = 0;
static int sd;

void usr2_handler(int s) {
    return;
}

int add_1_sever(void) {
    int slot;
    pid_t pid;

    if (idle_count + busy_count >= MAXCLIENT) {
        return -1;
    }

    for (slot = 0; slot < MAXCLIENT; slot++) {
        if (serverpool[slot].pid == -1) {
            break;
        }
    }
    serverpool[slot].state = STATE_IDLE;
    pid = fork();
    if (pid < 0) {
        perror("fork()");
        exit(1);
    }

    if (pid == 0) {
        server_job(sd, slot);
        exit(0);
    }
    serverpool[slot].pid = pid;
    idle_count++;

    return 0;
}

void server_job(int sd, int slot) {
    int client_fd;
    pid_t ppid;
    struct sockaddr_in clientAddr;
    char ipstr[INET_ADDRSTRLEN];

    socklen_t clientAddrLen = sizeof(clientAddr);
    ppid = getppid();
    while (1) {
        serverpool[slot].state = STATE_IDLE;
        kill(ppid, SIG_NOTIFY);
        client_fd = accept(sd, (struct sockaddr *) &clientAddr, &clientAddrLen);

        if (client_fd < 0) {
            if (errno == EINTR || errno == EAGAIN) {
                continue;
            }
            perror("accept()");
            exit(1);
        }
        serverpool[slot].state = STATE_BUSY;
        kill(ppid, SIG_NOTIFY);
        inet_ntop(AF_INET, &clientAddr.sin_addr, ipstr, INET_ADDRSTRLEN);
        printf("[%d]:client :%s: %d\n", getpid(), ipstr, ntohs(clientAddr.sin_port));
        send(client_fd, "hello", strlen("hello"), 0);

        sleep(300);
        close(client_fd);
    }
    close(sd);
}

int del_1_server(void) {
    int i;
    if (idle_count == 0) {
        return -1;
    }

    for (i = 0; i < MAXCLIENT; i++) {
        if (serverpool[i].pid != -1 && serverpool[i].state == STATE_IDLE) {
            kill(serverpool[i].pid, SIGTERM);
            serverpool[i].pid = -1;
            idle_count--;
            break;
        }
    }
    return 0;
}

int scan_pool() {
    int i, idle = 0, busy = 0;

    for (i = 0; i < MAXCLIENT; i++) {
        if (serverpool[i].pid == -1) {
            continue;
        }
        // kill 发0号信号，检查进程是否存在
        if (kill(serverpool[i].pid, 0)) {
            serverpool[i].pid = -1;
            continue;
        }
        if (serverpool[i].state == STATE_IDLE) {
            idle++;
        } else if (serverpool[i].state == STATE_BUSY) {
            busy++;
        } else {
            fprintf(stderr, "unknown state.\n");
            abort();
        }
    }
    idle_count = idle;
    busy_count = busy;
    return 0;
}

int main() {
    int ret, i;
    struct sockaddr_in myend;
    struct sigaction sa, osa;
    sigset_t set, oset;
    int val;

    // 回收子进程
    // 信号到来时，忽略掉信号
    sa.sa_handler = SIG_IGN;
    // 信号屏蔽字，可以当作容量是1的信号队列
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_NOCLDWAIT;
    sigaction(SIGCHLD, &sa, &osa);

    //
    sa.sa_handler = usr2_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIG_NOTIFY, &sa, &osa);


    // 先屏蔽掉信号，将到来的信号
    sigemptyset(&set);
    sigaddset(&set, SIG_NOTIFY);
    sigprocmask(SIG_BLOCK, &set, &oset);


    serverpool = mmap(NULL, sizeof(struct server_st) * MAXCLIENT, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS,
                      -1, 0);
    if (serverpool == MAP_FAILED) {
        perror("mmap()");
        exit(1);
    }

    for (i = 0; i < MAXCLIENT; i++) {
        serverpool[i].pid = -1;
    }

    val = 1;
    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0) {
        perror("socket()");
        exit(1);
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(SERVERPORT);

    if (bind(sd, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        perror("bind()");
        exit(1);
    }

    if (listen(sd, 10) == -1) {
        perror("listen()");
        exit(1);
    }

    for (i = 0; i < MINSPARSESERVERS; i++) {
        add_1_sever();
    }

    while (1) {
        // 将当前的信号屏蔽字恢复成oset，然后pause()等待信号到来
        // 设置屏蔽字打开
        sigsuspend(&oset);
        /*
         sigprocmask(SIG_SETMASK,&oset,NULL);
         pause();
         */
        scan_pool();

        if (idle_count > MAXSPARSESERVERS) {
            for (i = 0; i < (idle_count - MAXSPARSESERVERS); i++) {
                del_1_server();
            }
        } else if (idle_count < MINSPARSESERVERS) {
            for (i = 0; i < (MINSPARSESERVERS - idle_count); i++) {
                add_1_sever();
            }
        }

        for (i = 0; i < MAXCLIENT; i++) {
            if (serverpool[i].pid == -1) {
                putchar(' ');
            } else {
                if (serverpool[i].state == STATE_IDLE) {
                    putchar('.');
                } else {
                    putchar('x');
                }
            }
        }
        putchar('\n');
    }
    sigprocmask(SIG_SETMASK, &oset, NULL);
    exit(0);
}