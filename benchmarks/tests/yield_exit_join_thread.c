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
	puts("finished thread 2");
	pthread_exit(NULL);
}

// run by TID: 1
void thread1() {
	puts("starting thread 1");
    pthread_yield();
	puts("finished thread 1");
}


/*
 * Test pthread yield as well as join and exit
 * should print out:
 *	"starting thread 1"
 *  "starting thread 2"
 *  "finished thread 2"
 *  "finished thread 1"
 */
int main(int argc, char **argv) {
	pthread_t tid;

	pthread_create(&tid, NULL, thread2, NULL);
	thread1();

	// thread 1 joining thread 2
    pthread_join(tid, NULL);
	pthread_exit(NULL);
	return 0;
}
