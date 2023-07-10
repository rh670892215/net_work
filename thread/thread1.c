#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *threadMain(void *arg);

int main() {
    pthread_t threadID;
    int pthreadParam = 5;

    if (pthread_create(&threadID, NULL, threadMain, (void *) &pthreadParam) != 0) {
        puts("thread create error");
        return 0;
    }

    sleep(10);
    puts("main end");
    return 0;
}

void *threadMain(void *arg) {
    int i;
    int count = *((int *) arg);
    for (i = 0; i < count; i++) {
        puts("thread running...");
        sleep(1);
    }
    return NULL;
}

// gcc thread1.c -o tr1 -lpthread