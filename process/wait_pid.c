#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {

    int status;
    pid_t pid;
    pid = fork();
    if (pid == 0) {
        sleep(15);
        return 1;
    } else {
        printf("child process pid is %d\n", pid);
        // waitpid 第一个参数传-1表示等待任何子进程的结束
        // 第二个参数与wait相同
        // 第三个参数传声明的常量WNOHANG,即使没有终止的子进程也不会进入阻塞状态，而是返回0退出函数。
        while (!waitpid(-1, &status, WNOHANG)) {
            sleep(3);
            printf("parent wait 3 second.\n");
        }

        if (WIFEXITED(status)) {
            printf("child process return %d\n", WEXITSTATUS(status));
        }
    }

    return 0;
}