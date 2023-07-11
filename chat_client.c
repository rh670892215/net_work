#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 100
#define NAME_SIZE 20

char name[NAME_SIZE] = "[default]";
char buf[BUF_SIZE];

void errorHandling(char *message);

void *sendMsg(void *arg);

void *recvMsg(void *arg);

int main(int argc, char *argv[]) {
    int clientSocket = socket(PF_INET, SOCK_STREAM, 0);

    sprintf(name, "[%s]", argv[3]);

    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = inet_addr("127.0.0.1");//argv[1]
    client_addr.sin_port = htons(atoi(argv[2]));

    if (connect(clientSocket, (struct sockaddr *) &client_addr, sizeof(client_addr)) == -1) {
        errorHandling("connect error");
    } else {
        puts("connected...");
    }

    pthread_t threadSendID, threadRecvID;

    pthread_create(&threadSendID, NULL, sendMsg, (void *) &clientSocket);
    pthread_create(&threadRecvID, NULL, recvMsg, (void *) &clientSocket);

    pthread_join(threadSendID, NULL);
    pthread_join(threadRecvID, NULL);

    close(clientSocket);
    return 0;
}

void errorHandling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void *sendMsg(void *arg) {
    int sock = *((int *) arg);
    char msg[NAME_SIZE + BUF_SIZE];
    while (1) {
        fgets(buf, BUF_SIZE, stdin);
        if (!strcmp(buf, "q\n") || !strcmp(buf, "Q\n")) {
            close(sock);
            exit(0);
        }
        sprintf(msg, "%s %s", name, buf);
        write(sock, msg, strlen(msg));
    }
    return NULL;
}

void *recvMsg(void *arg) {
    int sock = *((int *) arg);
    char msg[NAME_SIZE + BUF_SIZE];
    while (1) {
        int strLen = read(sock, msg, sizeof(msg) - 1);
        msg[strLen] = 0;
        fputs(msg, stdout);
    }
    return NULL;
}

// gcc chat_client.c -D_REENTRANT -o client -lpthread