#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/select.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
    int serverSocket = socket(PF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serverAddr;
    struct sockaddr_in clientAddr;

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(atoi("8000"));

    bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    listen(serverSocket, 5);

    fd_set socks, cpySocks;
    struct timeval timeout;
    int fdMax;
    FD_ZERO(&socks);
    FD_SET(serverSocket, &socks);
    fdMax = serverSocket + 1;

    while (1) {
        cpySocks = socks;
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;

        int listenRes = select(fdMax + 1, &cpySocks, 0, 0, &timeout);
        if (listenRes == 0) {
            continue;
        }
        if (listenRes == -1) {
            break;
        }

        int i;
        for (i = 0; i < fdMax + 1; i++) {
            if (FD_ISSET(i, &cpySocks)) {
                if (i == serverSocket) {
                    socklen_t clientAddrLen = sizeof(clientAddr);
                    int clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddr, &clientAddrLen);
                    FD_SET(clientSocket, &socks);
                    if (clientSocket > fdMax) {
                        fdMax = clientSocket;
                    }
                    printf("connect client %d\n", clientSocket);
                    continue;
                }
                char message[BUF_SIZE];
                int strLen = read(i, message, BUF_SIZE);
                if (strLen == 0) {
                    // 接收的数据是EOF
                    FD_CLR(i, &socks);
                    close(i);
                    printf("closed client %d\n", i);
                } else {
                    // 回声数据
                    write(i, message, strLen);
                }
            }
        }
    }
    close(serverSocket);
    return 0;
}