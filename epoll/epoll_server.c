//#ifdef __linux__
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/epoll.h>

#define BUF_SIZE 1024
#define EPOLL_SIZE 30

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

    struct epoll_event *epEvents;
    struct epoll_event event;

    int epfd = epoll_create(EPOLL_SIZE);

    epEvents = malloc(sizeof(struct epoll_event) * EPOLL_SIZE);
    event.events = EPOLLIN;
    event.data.fd = serverSocket;
    epoll_ctl(epfd, EPOLL_CTL_ADD, serverSocket, &event);

    while (1) {
        int eventNum = epoll_wait(epfd, epEvents, EPOLL_SIZE, -1);
        if (eventNum == -1) {
            break;
        }


        int i;
        for (i = 0; i < eventNum; i++) {
            if (epEvents[i].data.fd == serverSocket) {
                socklen_t clientAddrLen = sizeof(clientAddr);
                int clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddr, &clientAddrLen);

                event.events = EPOLLIN;
                event.data.fd = clientSocket;
                epoll_ctl(epfd, EPOLL_CTL_ADD, clientSocket, &event);
                printf("connect client %d\n", clientSocket);
                continue;
            }

            char message[BUF_SIZE];
            int strLen = read(epEvents[i].data.fd, message, BUF_SIZE);
            if (strLen == 0) {
                // 收到EOF
                epoll_ctl(epfd, EPOLL_CTL_DEL, epEvents[i].data.fd, NULL);
                close(epEvents[i].data.fd);
                printf("closed client %d\n", epEvents[i].data.fd);
            } else {
                // 回声数据
                write(epEvents[i].data.fd, message, strLen);
            }

        }
    }
    close(serverSocket);
    close(epfd);
    return 0;
}

//#endif