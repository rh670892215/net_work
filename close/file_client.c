#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
    FILE *fp;

    int clientSocket = socket(PF_INET, SOCK_STREAM, 0);

    struct sockaddr_in clientAddr;
    memset(&clientAddr, 0, sizeof(clientAddr));
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    clientAddr.sin_port = htons(atoi("8000"));

    connect(clientSocket, (struct sockaddr *) &clientAddr, sizeof(clientAddr));

    fp = fopen("receive.dat", "wb");
    char message[BUF_SIZE];
    int readLen;
    while ((readLen = read(clientSocket, message, BUF_SIZE)) != 0) {
        fwrite((void *) message, 1, readLen, fp);
    }

    shutdown(clientSocket, SHUT_RD);

    write(clientSocket, "got it", 7);
    fclose(fp);
    close(clientSocket);
    return 0;
}