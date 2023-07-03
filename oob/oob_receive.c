#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>

#define BUF_SIZE 1024

void errorHandling(char *message);

void urgHandle(int sig);

int serverSocket;
int clientSocket;

int main(int argc, char *argv[]) {
    serverSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        errorHandling("socket() error");
    }

    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));

    if (bind(serverSocket, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        errorHandling("bind error");
    }
    if (listen(serverSocket, 5) == -1) {
        errorHandling("listen error");
    }

    struct sigaction act;
    act.sa_handler = urgHandle;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    socklen_t client_addr_size = sizeof(client_addr);
    clientSocket = accept(serverSocket, (struct sockaddr *) &client_addr, &client_addr_size);

    // 多个进程可以共同拥有 1个套接字的文件描述符,处理SIGURG信号时必须指定处理信号的进程
    // 因此,将clientSocket文件描述符的拥有者（owner）设置为当前进程的进程ID（PID）,以便在接收到指定信号时通知当前进程。
    fcntl(clientSocket, F_SETOWN, getpid());

    int state = sigaction(SIGURG, &act, 0);

    int strLen;
    char buf[BUF_SIZE];
    while ((strLen = recv(clientSocket, buf, BUF_SIZE - 1, 0)) != 0) {
        if (strLen == -1)
            continue;
        buf[strLen] = 0;
        puts(buf);
    }

    close(clientSocket);
    close(serverSocket);
    return 0;
}

void urgHandle(int sig) {
    char buf[BUF_SIZE];
    int strLen = recv(clientSocket, buf, BUF_SIZE - 1, MSG_OOB);
    buf[strLen] = 0;
    printf("oob msg %s\n", buf);
}

void errorHandling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}