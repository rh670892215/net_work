#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>

#define BUF_SIZE 1024

void errorHandling(char *message);

void childProcessHandle(int sig);

int main(int argc, char *argv[]) {
    int serverSocket = socket(PF_INET, SOCK_STREAM, 0);
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
    char message[BUF_SIZE];

    struct sigaction act;
    act.sa_handler = childProcessHandle;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    sigaction(SIGCHLD, &act, 0);

    while (1) {
        socklen_t client_addr_len = sizeof(client_addr);
        int clientSocket = accept(serverSocket, (struct sockaddr *) &client_addr, &client_addr_len);
        if (clientSocket == -1) {
            continue;
        }
        puts("server accept success....");
        pid_t pid = fork();
        if (pid == 0) {
            // 子进程处理逻辑
            close(serverSocket);
            int strLen;
            while ((strLen = read(clientSocket, &message, BUF_SIZE)) != 0)
                write(clientSocket, &message, strLen);
            close(clientSocket);
            puts("client disconnectd....");
            return 0;
        } else {
            close(clientSocket);
        }
    }
    close(serverSocket);
    return 0;
}

void childProcessHandle(int sig) {
    int status;
    pid_t pid = waitpid(-1, &status, WNOHANG);
    if (WIFEXITED(status)) {
        printf("child process exit.pid :[%d],return :[%d]", pid, WEXITSTATUS(status));
    }
}


void errorHandling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}