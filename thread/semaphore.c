#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

void *read(void *arg);

void *write(void *arg);

static int num;
static sem_t sem1;
static sem_t sem2;

int main() {
    pthread_t threadID1, threadID2;
    sem_init(&sem1, 0, 0);
    sem_init(&sem2, 0, 1);

    pthread_create(&threadID1, NULL, read, NULL);
    pthread_create(&threadID2, NULL, write, NULL);

    pthread_join(threadID1, NULL);
    pthread_join(threadID2, NULL);

    sem_destroy(&sem1);
    sem_destroy(&sem2);

    return 0;
}

void *read(void *arg) {
    int i;
    for (i = 0; i < 5; i++) {
        sem_wait(&sem2);
        scanf("%d", &num);
        sem_post(&sem1);
    }

    return NULL;
}

void *write(void *arg) {
    int i;
    int sum = 0;
    for (i = 0; i < 5; i++) {
        sem_wait(&sem1);
        sum += num;
        sem_post(&sem2);
    }
    printf("sum is %d\n", sum);
    return NULL;
}

