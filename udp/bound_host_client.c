#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int clientSocket = socket(PF_INET, SOCK_DGRAM, 0);

    char message1[] = "hello";
    char message2[] = "world";
    char message3[] = "look";

    struct sockaddr_in clientAddr;
    memset(&clientAddr, 0, sizeof(clientAddr));
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    clientAddr.sin_port = htons(atoi("8000"));

    sendto(clientSocket, message1, sizeof(message1), 0, (struct sockaddr *) &clientAddr, sizeof(clientAddr));
    sendto(clientSocket, message2, sizeof(message2), 0, (struct sockaddr *) &clientAddr, sizeof(clientAddr));
    sendto(clientSocket, message3, sizeof(message3), 0, (struct sockaddr *) &clientAddr, sizeof(clientAddr));

    close(clientSocket);
    return 0;
}