#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

#define BUF_SIZE 1024

void errorHandling(char *message);

void readMessage(int sock, char *message);

void writeMessage(int sock, char *message);

int main(int argc, char *argv[]) {
    int clientSocket = socket(PF_INET, SOCK_STREAM, 0);

    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = inet_addr("127.0.0.1");//argv[1]
    client_addr.sin_port = htons(atoi(argv[2]));

    if (connect(clientSocket, (struct sockaddr *) &client_addr, sizeof(client_addr)) == -1) {
        errorHandling("connect error");
    } else {
        puts("connected...");
    }

    char message[BUF_SIZE];
    pid_t pid = fork();
    if (pid == 0) {
        writeMessage(clientSocket, message);
    } else {
        readMessage(clientSocket, message);
    }

    close(clientSocket);
    return 0;
}

void readMessage(int sock, char *message) {
    while (1) {
        int strLen = read(sock, message, BUF_SIZE);
        if (strLen == 0) {
            return;
        }
        message[strLen] = 0;
        printf("read message is %s", message);
    }
}

void writeMessage(int sock, char *message) {
    while (1) {
        fgets(message, BUF_SIZE, stdin);
        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n")) {
            // 注意这里,通过调用shutdown(sock, SHUT_WR)，可以向服务器发送一个特定的信号，告知服务器已经没有更多的数据要发送了
            // 在return之后会执行close(clientSocket)关闭整个套接字，但关闭整个套接字并不会发送一个明确的信号给服务器端，告知数据传输已经完成。
            shutdown(sock, SHUT_WR);
            return;
        }
        write(sock, message, BUF_SIZE);
    }
}

void errorHandling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}