#include <stdio.h>
#include <pthread.h>

void *threadMain(void *arg);

int sum = 0;

int main() {

    pthread_t threadID1, threadID2;
    int range1[] = {1, 5};
    int range2[] = {6, 10};

    pthread_create(&threadID1, NULL, threadMain, (void *) range1);
    pthread_create(&threadID2, NULL, threadMain, (void *) range2);

    pthread_join(threadID1, NULL);
    pthread_join(threadID2, NULL);

    printf("sum is %d", sum);
    return 0;
}

void *threadMain(void *arg) {
    int start = ((int *) arg)[0];
    int end = ((int *) arg)[1];

    while (start <= end) {
        sum += start;
        start++;
    }
    return NULL;
}

// gcc thread3.c -D_REENTRANT -o tr3 -lpthread