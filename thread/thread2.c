#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

void *threadMain(void *arg);

int main() {
    void *threadRes;
    int threadParam = 5;
    pthread_t threadID;

    if (pthread_create(&threadID, NULL, threadMain, (void *) &threadParam) != 0) {
        puts("thread create error");
        return -1;
    }

    if (pthread_join(threadID, &threadRes) != 0) {
        puts("thread join error");
        return -1;
    }

    printf("thread return %s", (char *) threadRes);
    free(threadRes);
    return 0;
}

void *threadMain(void *arg) {
    int i;
    int count = *((int *) arg);
    char *res = (char *) malloc(sizeof(char) * 20);
    strcpy(res, "hello world\n");

    for (i = 0; i < count; i++) {
        puts("thread running...");
        sleep(1);
    }
    return (void *) res;
}
