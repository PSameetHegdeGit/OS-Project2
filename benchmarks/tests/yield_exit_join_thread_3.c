#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "../../mypthread.h"

extern tcb *runQ_head;
extern tcb *runQ_tail;

// Run by TID: 2
void* thread2(void *arg) {
	puts("starting thread 2");
	int i;
    int j = 0;
	for (i = 0; i < 1; i++) {
        j++;
    }

    // thread 2 joining thread 1
    int retVal;
    pthread_join(1, (void**)&retVal);
    printf("return value: %d\n", retVal);

    puts("finished thread 2");
	pthread_exit(NULL);
}

// run by TID: 1
void thread1() {
	puts("starting thread 1");
	int i;
    int j = 0;
	for (i = 0; i < 99999999; i++) {
        j++;
    }
	puts("finished thread 1");
    pthread_exit((void*) -1);
}

/*
 * Thread1 (the initial thread) takes long time to run and the child thread joins it
 * should print out "return value: -1"
 */
int main(int argc, char **argv) {
	pthread_t tid;
	pthread_create(&tid, NULL, thread2, NULL);

	thread1();
	return 0;
}
