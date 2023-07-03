#include <stdio.h>
#include <unistd.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
    char str1[] = "hello world";
    char str2[] = "welcome in";
    char message[BUF_SIZE];
    int fds[2];

    pipe(fds);

    pid_t pid = fork();
    if (pid == 0) {
        write(fds[1], str1, sizeof(str1));
        // 下面这个"sleep(2);"的代码若注释掉,会导致程序无限等待,原因如下:
        // 数据进入管道后成为无主数据.也就是通过read函数先读取数据的进程将得到数据.即使该进程将数据传到了管道
        // 因此,注释掉"sleep(2);",接下来子进程将读回自己向管道发送的数据.
        // 结果,父进程调用read函数后将元限期等待数据进入管道.
        sleep(2);
        read(fds[0], message, BUF_SIZE);
        printf("child process read message : %s\n", message);
    } else {
        read(fds[0], message, BUF_SIZE);
        printf("parent process read message : %s\n", message);
        write(fds[1], str2, sizeof(str2));
        sleep(3);
    }
}