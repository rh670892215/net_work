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


    socklen_t client_addr_size = sizeof(client_addr);
    clientSocket = accept(serverSocket, (struct sockaddr *) &client_addr, &client_addr_size);

    int strLen;
    char buf[BUF_SIZE];
    while (1) {
        // 同时设置MSG-PEEK选项和MSG-DONTWAIT选项,以验证输入缓冲中是否存在接收的数据.
        // 设置MSG-PEEK选项并调用recv函数时，即使读取了输入缓冲的数据也不会删除.
        // 因此,该选项通常与MSG-DONTWAIT合作,用于调用以非阻塞方式验证待读数据存在与否的函数
        strLen = recv(clientSocket, buf, BUF_SIZE - 1, MSG_PEEK | MSG_DONTWAIT);
        if (strLen > 0)
            break;
    }
    buf[strLen] = 0;
    puts(buf);

    strLen = recv(clientSocket, buf, BUF_SIZE - 1, MSG_PEEK | MSG_DONTWAIT);
    buf[strLen] = 0;
    puts(buf);

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