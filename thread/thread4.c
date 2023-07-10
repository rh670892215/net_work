#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_THREAD 100

void *inc(void *arg);

void *dec(void *arg);

long long num = 0;

int main() {
    pthread_t threadIDs[NUM_THREAD];
    int i;
    for (i = 0; i < NUM_THREAD; i++) {
        if (i % 2 == 0) {
            pthread_create(&threadIDs[i], NULL, inc, NULL);
        } else {
            pthread_create(&threadIDs[i], NULL, dec, NULL);
        }
    }

    for (i = 0; i < NUM_THREAD; i++) {
        pthread_join(threadIDs[i], NULL);
    }
    printf("num is %lld\n", num);
    return 0;
}

void *inc(void *arg) {
    int i;
    for (i = 0; i < 10000000; i++) {
        num++;
    }
    return NULL;
}

void *dec(void *arg) {
    int i;
    for (i = 0; i < 10000000; i++) {
        num--;
    }
    return NULL;
}