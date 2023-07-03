#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char *argv[]) {

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    int state;
    int receiveBuf;
    int sendBuf;
    socklen_t bufLen;

    bufLen = sizeof(sendBuf);
    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void *) &sendBuf, &bufLen);
    bufLen = sizeof(receiveBuf);
    state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void *) &receiveBuf, &bufLen);

    printf("input buf size: %d\n", receiveBuf);
    printf("output buf size: %d\n", sendBuf);

    return 0;
}