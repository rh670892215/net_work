#include <stdio.h>
#include <unistd.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
    int fds1[2];
    int fds2[2];

    char str1[] = "hello world";
    char str2[] = "welcome in";
    char message[BUF_SIZE];

    pipe(fds1);
    pipe(fds2);

    pid_t pid = fork();
    if (pid == 0) {
        read(fds1[0], message, BUF_SIZE);
        printf("child process read message : %s\n", message);
        write(fds2[1], str2, sizeof(str2));
    } else {
        write(fds1[1], str1, sizeof(str1));
        read(fds2[0], message, BUF_SIZE);
        printf("parent process read message : %s\n", message);
    }
    return 0;
}