#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    pid_t pid;
    pid = fork();
    if (pid == 0) {
        puts("here is child process");
    } else {
        printf("child pid is %d\n", pid);
        sleep(30);
    }

    if (pid == 0) {
        puts("child process end");
    } else {
        puts("parent process end");
    }

    return 0;
}