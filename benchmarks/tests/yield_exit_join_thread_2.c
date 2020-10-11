#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "../../mypthread.h"

extern tcb *runQ_head;
extern tcb *runQ_tail;

// Run by TID: 2
void* thread1(void *arg) {
	puts("starting thread 2");
	int i;
    int j = 0;
	for (i = 0; i < 99999999; i++) {
        j++;
    }
	puts("finished thread 2");
	pthread_exit((void*) -1);
}

// run by TID: 1
void thread0() {
	puts("starting thread 1");
	int i;
    int j = 0;
	for (i = 0; i < 10; i++) {
        j++;
    }
	puts("finished thread 1");
}

/*
 * Thread2 (the child thread) takes long time to run and the parent thread joins it
 * should print out:
 *	"starting thread 1"
 *  "starting thread 2"
 *  "finished thread 2"
 *  "finished thread 1"
 *  "return value: -1"
 */
int main(int argc, char **argv) {
	pthread_t tid;

	pthread_create(&tid, NULL, thread1, NULL);
	thread0();

	// thread 1 joining thread 2
	int retVal;
    pthread_join(tid, (void**)&retVal);
	printf("return value: %d\n", retVal);

	pthread_exit(NULL);
	return 0;
}
