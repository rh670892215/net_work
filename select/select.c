#include <stdio.h>
#include <sys/select.h>
#include <unistd.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[]) {

    char buf[BUF_SIZE];

    fd_set reads;
    FD_ZERO(&reads);
    FD_SET(0, &reads);

    struct timeval timeout;


    while (1) {
        // 调用select函数后,除发生变化的文件描述符对应位外，剩下的所有位将初始化为0.
        // 因此,为了记住初始值,必须经过这种复制过程.这是使用select函数的通用方法.
        fd_set tmp = reads;
        // 调用select函数后,结构体timeval的值将被替换为超时前剩余时间.因此,调用select函数前,每次都/需要初始化timeval结构体变量
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        int res = select(1, &tmp, 0, 0, &timeout);
        if (res == 0) {
            puts("timeout");
            return 0;
        } else if (res == -1) {
            puts("select error");
            return 0;
        } else {
            if (FD_ISSET(0, &tmp)) {
                // stdin的文件描述符是0
                int strLen = read(0, buf, BUF_SIZE);
                buf[strLen] = 0;
                puts(buf);
            }
        }
    }
    return 0;
}