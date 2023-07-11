#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define BUF_SIZE 1024
#define MAX_CLNT 256

int clientCnt = 0;
int clientSockets[MAX_CLNT];

pthread_mutex_t mutex;

void errorHandling(char *msg);

void *threadMain(void *arg);

void sendMessage(char *message, int len);

int main(int argc, char *argv[]) {
    int serverSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        errorHandling("socket() error");
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

    pthread_mutex_init(&mutex, NULL);

    while (1) {
        socklen_t clientAddrSize = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddr, &clientAddrSize);

        pthread_mutex_lock(&mutex);
        clientSockets[clientCnt++] = clientSocket;
        pthread_mutex_unlock(&mutex);

        pthread_t threadID;
        pthread_create(&threadID, NULL, threadMain, (void *) &clientSocket);
        pthread_detach(threadID);
        printf("connected client IP is %s\n", inet_ntoa(clientAddr.sin_addr));
    }
    close(serverSocket);
    return 0;
}

void errorHandling(char *msg) {
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}

void *threadMain(void *arg) {
    int sock = *((int *) arg);
    int strLen = 0;
    char message[BUF_SIZE];

    while ((strLen = read(sock, message, sizeof(message))) != 0) {
        sendMessage(message, strLen);
    }

    // 接收到消息是0，证明已经断开连接
    int i;
    pthread_mutex_lock(&mutex);
    for (i = 0; i < clientCnt; i++) {
        if (sock == clientSockets[i]) {
            while (i < clientCnt - 1) {
                clientSockets[i] = clientSockets[i + 1];
                i++;
            }
            break;
        }
    }
    clientCnt--;
    pthread_mutex_unlock(&mutex);
    close(sock);
    return NULL;
}

void sendMessage(char *message, int len) {
    int i;
    pthread_mutex_lock(&mutex);
    for (i = 0; i < clientCnt; i++) {
        write(clientSockets[i], message, len);
    }
    pthread_mutex_unlock(&mutex);
}

// gcc chat_server.c -D_REENTRANT -o server -lpthread