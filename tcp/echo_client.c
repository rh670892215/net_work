#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

#define BUF_SIZE 1024

void errorHandling(char *message);

int main(int argc, char *argv[]) {
    int clientSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        errorHandling("socket() error");
    }

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

    while (1) {
        fputs("input message(q to quit): ", stdout);
        fgets(message, BUF_SIZE, stdin);

        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;
        int writeLen = write(clientSocket, message, strlen(message));
        int readLen = 0;
        while (readLen < writeLen) {
            int strLen = read(clientSocket, &message[readLen], BUF_SIZE - 1);
            readLen += strLen;
        }
        message[readLen] = 0;
        printf("message is %s", message);
    }
    close(clientSocket);
    return 0;
}

void errorHandling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}