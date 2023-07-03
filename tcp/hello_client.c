#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

void errorHandling(char *message);

int main(int argc, char *argv[]) {
    int clientSocket;
    clientSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        errorHandling("socket() error");
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    // inet_addr: 将字符串形式的 IP 地址转换为网络字节序形式的32位整数型数据
    serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
    serverAddr.sin_port = htons(atoi(argv[2]));

    // 与服务端一致,客户端也需要分配IP和端口号
    // 客户端在调用connect函数时在os的内核中使用计算机的IP作为其IP,对于端口是随机分配的
    if (connect(clientSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1) {
        errorHandling("connect error");
    }

    char message[30];
    int strLen = read(clientSocket, message, sizeof(message) - 1);
    if (strLen == -1) {
        errorHandling("read error");
    }

    printf("message is:%s\n", message);
    close(clientSocket);
    return 0;
}


void errorHandling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}