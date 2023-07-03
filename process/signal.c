#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void keyControl(int sig);

void timeout(int sig);

int main(int argc, char *argv[]) {
    signal(SIGINT, keyControl);
    signal(SIGALRM, timeout);
    alarm(2);

    int i;
    for (i = 0; i < 3; i++) {
        // 这里看程序本应阻塞300s,但是实际执行过程中不到10s程序就会执行结束,原因如下:
        // 发生信号时将唤醒由于调用 sleep函数而进入阻塞状态的进程,而进程一旦被唤醒就不会再进入睡眠状态(即使sleep中的时间还未到);
        // 因此整个程序10s还不到就会运行完毕;
        puts("wait...");
        sleep(100);
    }
    return 0;
}

void keyControl(int sig) {
    if (sig == SIGINT) {
        puts("input ctr + c");
    }
}

void timeout(int sig) {
    if (sig == SIGALRM) {
        puts("timeout...");
    }
    alarm(2);
}