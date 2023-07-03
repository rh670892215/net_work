#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define BUF_SIZE 1024

void errorHandling(char *message);

int main(int agrc, char *argv[]) {
    int serverSocket = socket(PF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in serverAddr;
    struct sockaddr_in clientAddr;

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(atoi("8000"));

    if (bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1) {
        errorHandling("bind error");
    }


    while (1) {
        char message[BUF_SIZE];
        socklen_t clientAddrLen = sizeof(clientAddr);
        // recvfrom 接收数据
        // sock:用于接收数据的UDP套接字;
        // buff:保存接收数据的缓冲地址值;
        // nbytes:可接收的最大字节数,无法超过参数buff所指的缓存大小;
        // flags:可选项参数,没有传0;
        // from:存有发送端地址信息的sockaddr结构体变量的地址;
        // addrlen:保存from结构体变量长度;
        int strLen = recvfrom(serverSocket, message, BUF_SIZE, 0, (struct sockaddr *) &clientAddr, &clientAddrLen);
        // sendto 发送数据;执行步骤:1.向UDP套接字注册目标IP与端口号;2.传输数据;3.删除UDP套接字中注册的目标地址信息;
        // sock:用于传输数据的UDP套接字;
        // buff:保存待传输数据的缓冲地址值;
        // nbytes:待传输的数据长度;
        // flags:可选项参数,没有传0;
        // to:存有目标地址信息的sockaddr结构体变量的地址;
        // addrlen:保存参数to结构体变量长度;
        sendto(serverSocket, message, strLen, 0, (struct sockaddr *) &clientAddr, clientAddrLen);
    }
    close(serverSocket);
    return 0;
}

void errorHandling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}