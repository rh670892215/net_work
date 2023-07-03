#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    pid_t pid;
    pid = fork();
    int status;
    if (pid == 0) {
        return 1;
    } else {
        printf("child one process pid is [%d]\n", pid);
        pid = fork();
        if (pid == 0) {
            return 2;
        } else {
            printf("child two process pid is [%d]\n", pid);
            // 调用wait函数消灭僵尸进程的,调用wait函数时，如果没有己终止的子进程,那么程序将阻塞(Blocking)直到有子进程终止
            wait(&status);
            if (WIFEXITED(status)) {
                printf("child one return %d\n", WEXITSTATUS(status));
            }

            wait(&status);
            if (WIFEXITED(status)) {
                printf("child two return %d\n", WEXITSTATUS(status));
            }
            sleep(30);
        }
    }
    return 0;
}