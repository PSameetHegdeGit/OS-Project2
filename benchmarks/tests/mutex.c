#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../../mypthread.h"

pthread_t tid[2];
int counter;
pthread_mutex_t lock;

void* trythis(void* arg) {
    pthread_mutex_lock(&lock);

    unsigned long i = 0;
    counter += 1;
    printf("Job %d has started\n", counter);

    for (i = 0; i < (0xFFFFFFFF); i++);

    printf("Job %d has finished\n", counter);

    pthread_mutex_unlock(&lock);
    pthread_exit(NULL);

    return NULL;
}

int main(void) {
    int i = 0;
    int error;

    pthread_mutex_init(&lock, NULL) != 0)

    while (i < 2) {
        pthread_create(&(tid[i]), NULL, &trythis, NULL);
        i++;
    }

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_mutex_destroy(&lock);
    pthread_exit(NULL);

    return 0;
}
