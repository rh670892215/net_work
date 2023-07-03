#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
    int receiveSocket = socket(PF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(atoi(argv[2]));

    bind(receiveSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

    struct ip_mreq joinAdr;
    joinAdr.imr_multiaddr.s_addr = inet_addr(argv[1]);
    joinAdr.imr_interface.s_addr = htonl(INADDR_ANY);

    setsockopt(receiveSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *) &joinAdr, sizeof(joinAdr));

    while (1) {
        char buf[BUF_SIZE];
        int strLen = recvfrom(receiveSocket, buf, BUF_SIZE - 1, 0, NULL, 0);
        if (strLen < 0)
            break;
        buf[strLen] = 0;
        puts(buf);
    }

    return 0;
}