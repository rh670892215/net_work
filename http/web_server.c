#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define BUF_SIZE 1024
#define SMALL_BUF 100

void errorHandling(char *message);

void *handler(void *arg);

char *getContentType(char *str);

void sendError(FILE *fp);

void sendData(FILE *fp, char *contentType, char *fileName);

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

    pthread_t threadID;
    while (1) {
        socklen_t clientAddrSize = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddr, &clientAddrSize);

        printf("client connected %s %d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
        pthread_create(&threadID, NULL, handler, (void *) &clientSocket);
        pthread_detach(threadID);
    }
    close(serverSocket);
    return 0;
}

void *handler(void *arg) {
    // GET /index.html HTTP/1.1
    int sock = *((int *) arg);
    char request[SMALL_BUF];

    FILE *readClient = fdopen(sock, "r");
    FILE *writeClient = fdopen(dup(sock), "w");

    // 读取请求信息
    fgets(request, SMALL_BUF, readClient);
    if (strstr(request, "HTTP/") == NULL) {
        sendError(writeClient);
        fclose(readClient);
        fclose(writeClient);
        return NULL;
    }
    // 解析请求信息  exp:GET /index.html HTTP/1.1
    // 解析请求方法
    char method[10];
    strcpy(method, strtok(request, " /"));

    // 解析请求文件,strtok传入NULL,表示从上一次的分割结果继续分割
    char fileName[30];
    strcpy(fileName, strtok(NULL, " /"));

    char contentType[20];
    strcpy(contentType, getContentType(fileName));

    if (strcmp(method, "GET") != 0) {
        sendError(writeClient);
        fclose(readClient);
        fclose(writeClient);
        return NULL;
    }
    fclose(readClient);
    sendData(writeClient, contentType, fileName);
}

char *getContentType(char *str) {
    char fileName[SMALL_BUF];
    char extension[SMALL_BUF];
    strcpy(fileName, str);
    strtok(fileName, ".");
    strcpy(extension, strtok(NULL, "."));

    if (!strcmp(extension, "html") || !strcmp(extension, "htm"))
        return "text/html";// 网页文档类型
    else
        return "text/plain";// 纯文档类型
}

void sendError(FILE *fp) {
    char protocol[] = "HTTP/1.0 400 Bad Request\r\n";
    char server[] = "Server:Linux Web Server \r\n";
    char cntLen[] = "Content-length:2048\r\n";
    char contentType[] = "Content-type:text/html\r\n\r\n";
    char content[] = "<html><head><title>NETWORK</title></head>"
                     "<body><font size=+5><br>发生错误！ 查看请求文件名和请求方式!"
                     "</font></body></html>";
    fputs(protocol, fp);
    fputs(server, fp);
    fputs(cntLen, fp);
    fputs(contentType, fp);
    fputs(content, fp);
    fflush(fp);
}

void sendData(FILE *fp, char *contentType, char *fileName) {
    // 构建请求头
    char protocol[] = "HTTP/1.0 200 OK\r\n";
    char server[] = "Server:Linux Web Server \r\n";
    char cntLen[] = "Content-length:2048\r\n";
    char cntType[SMALL_BUF];
    sprintf(cntType, "Content-type:%s\r\n\r\n", contentType);

    char buf[BUF_SIZE];
    FILE *sendFile = fopen(fileName, "r");
    if (sendFile == NULL) {
        sendError(fp);
        return;
    }
    //传输头信息
    fputs(protocol, fp);
    fputs(server, fp);
    fputs(cntLen, fp);
    fputs(cntType, fp);

    while (fgets(buf, BUF_SIZE, sendFile) != NULL) {
        fputs(buf, fp);
        fflush(fp);
    }
    fflush(fp);
    fclose(fp);
}

void errorHandling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
