#include <unistd.h>
#include <stdio.h>

int globalVal = 10;

int main(int argc, char *argv[]) {
    int localVal = 25;
    pid_t pid;
    pid = fork();
    if (pid == 0) {
        // 子进程
        localVal++;
        globalVal++;
    } else {
        // 父进程
        localVal--;
        globalVal--;
    }

    if (pid == 0) {
        printf("child proc localVal :[%d],globalVal is :[%d]\n", localVal, globalVal);
    } else {
        printf("parent proc localVal :[%d],globalVal is :[%d]\n", localVal, globalVal);
    }
    // 通过fork创建子进程后,父子进程拥有独立的内存结构,只是共用一套代码

    return 0;
}