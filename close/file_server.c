#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define BUF_SIZE 30

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

    socklen_t clientAddrLen = sizeof(clientAddr);
    int clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddr, &clientAddrLen);

    // 读取文件
    char message[BUF_SIZE];
    FILE *fp;
    fp = fopen("file_server.c", "rb");

    while (1) {
        int readLen = fread((void *) message, 1, BUF_SIZE, fp);
        if (readLen < BUF_SIZE) {
            write(clientSocket, message, readLen);
            break;
        }
        write(clientSocket, message, BUF_SIZE);
    }

    // 关闭连接
    shutdown(clientSocket, SHUT_WR);
    read(clientSocket, message, BUF_SIZE);
    printf("message from client is %s:", message);

    fclose(fp);
    close(serverSocket);
    close(clientSocket);

    return 0;
}