#include <stdio.h>
#include <unistd.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
    int fds[2];
    char message[BUF_SIZE];
    char str[] = "hello world";
    // 创建管道,fds[0]:消息出口; fds[1]:消息入口;
    pipe(fds);

    pid_t pid = fork();
    if (pid == 0) {
        // 注意,子进程复制的并非管道，而是用于管道I/O的文件描述符
        write(fds[1], str, sizeof(str));
    } else {
        read(fds[0], message, BUF_SIZE);
        puts(message);
    }
    return 0;
}