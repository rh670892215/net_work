#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void timeout(int sig);

int main(int argc, char *argv[]) {
    struct sigaction act;
    act.sa_handler = timeout;
    // 将act结构体中的sa_mask成员设置为空信号集，表示在执行信号处理函数时不屏蔽任何信号。这意味着，在信号处理函数执行期间，可以接收和处理其他信号
    sigemptyset(&act.sa_mask);
    // 将act结构体中的sa_flags成员设置为0，表示不使用任何特殊的标志。这意味着，在信号处理函数执行期间，不会进行任何特殊的处理
    act.sa_flags = 0;

    sigaction(SIGALRM, &act, 0);

    alarm(2);
    int i;
    for (i = 0; i < 3; i++) {
        puts("wait...");
        sleep(100);
    }
    return 0;
}

void timeout(int sig) {
    if (sig == SIGALRM) {
        puts("timeout...");
    }
    alarm(2);
}