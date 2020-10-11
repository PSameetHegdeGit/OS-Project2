#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "../../mypthread.h"

extern tcb *runQ_head;
extern tcb *runQ_tail;

// Run by TID: 1
void* thread1(void *arg) {
	puts("starting thread 1");
	puts("finished thread 1");
	pthread_exit(NULL);
}

// run by TID: 0
void thread0() {
	puts("starting thread 0");
    pthread_yield();
	puts("finished thread 0");
}


/*
 * Test pthread yield as well as join and exit
 * should print out:
 *	"starting thread 0"
 *  "starting thread 1"
 *  "finished thread 1"
 *  "finished thread 0"
 */
int main(int argc, char **argv) {
	pthread_t tid;

	pthread_create(&tid, NULL, thread1, NULL);
	thread0();

	// thread 0 joining thread 1
    pthread_join(tid, NULL);
	pthread_exit(NULL);
	return 0;
}
