#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

void errorHandling(char *message);

int main(int argc, char *argv[]) {
    int clientSocket = socket(PF_INET, SOCK_STREAM, 0);

    struct sockaddr_in clientAddr;
    memset(&clientAddr, 0, sizeof(clientAddr));
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    clientAddr.sin_port = htons(atoi(argv[1]));

    if (connect(clientSocket, (struct sockaddr *) &clientAddr, sizeof(clientAddr)) == -1) {
        errorHandling("connect error");
    } else {
        printf("connect......");
    }

    write(clientSocket, "123", strlen("123"));

    close(clientSocket);
    return 0;
}

void errorHandling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}