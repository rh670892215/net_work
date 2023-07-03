#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define BUF_SIZE 1024
#define OPSZ 4

void errorHandling(char *message);

int calc(int num, int opnds[], char opt);

int main(int argc, char *argv[]) {
    int serverSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        errorHandling("socket error");
    }

    struct sockaddr_in serverAddr;
    struct sockaddr_in clientAddr;

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(atoi(argv[1]));

    if (bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1) {
        errorHandling("bind error");
    }

    if (listen(serverSocket, 5) == -1) {
        errorHandling("listen error");
    }

    int i;
    int clientSocket;
    int optCount = 0;
    for (i = 0; i < 5; i++) {
        socklen_t clientAddrLen = sizeof(clientAddr);
        clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddr, &clientAddrLen);
        if (clientSocket == -1) {
            errorHandling("accept error");
        } else {
            printf("connect success %d", i);
        }
        // 读取需要计算的数据个数
        // 操作数个数1字节
        read(clientSocket, &optCount, 1);
        int revLen = 0;
        char message[BUF_SIZE];
        // 一个操作数4字节,操作符1字节
        while ((OPSZ * optCount + 1) > revLen) {
            int strLen = read(clientSocket, &message[revLen], BUF_SIZE - 1);
            revLen += strLen;
        }
        int res = calc(optCount, (int *) &message, message[revLen - 1]);
        write(clientSocket, (char *) &res, sizeof(res));
        close(clientSocket);
    }

    close(serverSocket);
    return 0;
}

int calc(int num, int opnds[], char opt) {
    int result = opnds[0];
    int i;
    switch (opt) {
        case '+':
            for (i = 1; i < num; i++) result += opnds[i];
            break;
        case '-':
            for (i = 1; i < num; i++) result -= opnds[i];
            break;
        case '*':
            for (i = 1; i < num; i++) result *= opnds[i];
            break;
        case '/':
            for (i = 1; i < num; i++) result /= opnds[i];
            break;
        default:
            errorHandling("unDefine opt");
            break;
    }
    return result;
}


void errorHandling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}