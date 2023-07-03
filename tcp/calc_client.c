#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define BUF_SIZE 1024
#define OPSZ 4

void errorHandling(char *message);

int main(int argc, char *argv[]) {
    int clientSocket = socket(PF_INET, SOCK_STREAM, 0);

    struct sockaddr_in clientAddr;
    memset(&clientAddr, 0, sizeof(clientAddr));
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    clientAddr.sin_port = htons(atoi(argv[1]));


    if (connect(clientSocket, (struct sockaddr *) &clientAddr, sizeof(clientAddr)) == -1) {
        errorHandling("connect error");
    } else {
        printf("connect......");
    }

    int count;
    fputs("input count:", stdout);
    scanf("%d", &count);
    printf("confirm your input is %d", count);
    char message[BUF_SIZE];
    message[0] = (char) count;
    int i;
    for (i = 0; i < count; i++) {
        printf("input opnd[%d]", i);
        scanf("%d", (int *) &message[i * OPSZ + 1]);
        printf("confirm your input is %d", message[i * OPSZ + 1]);
    }

    fgetc(stdin);
    fputs("input opt", stdout);
    scanf("%c", &message[count * OPSZ + 1]);
    printf("confirm your input is %c", message[count * OPSZ + 1]);
    write(clientSocket, message, count * OPSZ + 2);
    int res;
    read(clientSocket, &res, sizeof(res));
    printf("res ids %d", res);

    close(clientSocket);
    return 0;
}


void errorHandling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}