#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

#define TTL 64
#define BUF_SIZE 1024

int main(int argc, char *argv[]) {

    int sendSocket = socket(PF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in sendAddr;
    memset(&sendAddr, 0, sizeof(sendAddr));
    sendAddr.sin_family = AF_INET;
    sendAddr.sin_addr.s_addr = inet_addr(argv[1]);
    sendAddr.sin_port = htons(atoi(argv[2]));

    int timeLive = TTL;
    setsockopt(sendSocket, IPPROTO_IP, IP_MULTICAST_TTL, (void *) &timeLive, sizeof(timeLive));

    FILE *fp = fopen("news.txt", "r");

    char buf[BUF_SIZE];
    while (!feof(fp)) {
        fgets(buf, BUF_SIZE, fp);
        sendto(sendSocket, buf, strlen(buf), 0, (struct sockaddr *) &sendAddr, sizeof(sendAddr));
        sleep(2);
    }

    fclose(fp);
    close(sendSocket);
    return 0;
}