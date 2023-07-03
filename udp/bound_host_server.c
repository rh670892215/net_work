#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define BUF_SIZE 1024

int main(int argc, char *agv[]) {
    int serverSocket = socket(PF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in serverAddr;
    struct sockaddr_in clientAddr;

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(atoi("8000"));

    bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    char message[BUF_SIZE];
    int i;
    for (i = 0; i < 3; i++) {
        sleep(2);
        socklen_t clientAddrLen = sizeof(clientAddr);
        recvfrom(serverSocket, message, BUF_SIZE, 0, (struct sockaddr *) &clientAddr, &clientAddrLen);

        printf("message %d is %s\n", i + 1, message);
    }

    close(serverSocket);
    return 0;
}