#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define BUF_SIZE 1024

void errorHandling(char *message);

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
    socklen_t client_addr_size = sizeof(client_addr);

    int clientSocket = accept(serverSocket, (struct sockaddr *) &client_addr, &client_addr_size);
    if (clientSocket == -1) {
        errorHandling("accept error");
    } else {
        printf("connect client \n");
    }
    FILE *readFd = fdopen(clientSocket, "r");
    FILE *writeFd = fdopen(dup(clientSocket), "w");

    fputs("hello world\n", writeFd);
    fputs("my name is bank\n", writeFd);
    fputs("nice to meet you\n", writeFd);
    fflush(writeFd);

    shutdown(fileno(writeFd), SHUT_WR);
    fclose(writeFd);

    fgets(message, BUF_SIZE, readFd);
    fputs(message, stdout);
    fclose(readFd);

    close(serverSocket);
    return 0;
}


void errorHandling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}