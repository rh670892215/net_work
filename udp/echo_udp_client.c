#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define BUF_SIZE 1024

void errorHandling(char *message);

int main(int argc, char *argv[]) {
    int clientSocket = socket(PF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in serverAddr;
    struct sockaddr_in clientAddr;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddr.sin_port = htons(atoi("8000"));

    char inputMessage[BUF_SIZE];
    char outputMessage[BUF_SIZE];
    while (1) {
        printf("input message(q/Q to exit):\n");
        fgets(inputMessage, sizeof(inputMessage), stdin);

        if (!strcmp(inputMessage, "q\n") || !strcmp(inputMessage, "Q\n")) {
            break;
        }

        // 这里没有bind函数,因为在调用sendto时os自动分配IP和端口号,IP使用逐句IP,端口号随机
        sendto(clientSocket, &inputMessage, strlen(inputMessage), 0, (struct sockaddr *) &serverAddr,
               sizeof(serverAddr));
        socklen_t clientLen = sizeof(clientAddr);
        int strLen = recvfrom(clientSocket, outputMessage, BUF_SIZE, 0, (struct sockaddr *) &clientAddr, &clientLen);
        outputMessage[strLen] = 0;
        printf("server output is %s", outputMessage);
    }
    return 0;
}

void errorHandling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}