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
	return NULL;
}

// run by TID: 0
void thread0() {
	puts("starting thread 0");
    pthread_yield();
	puts("finished thread 0");
	return -1;
}

/*
should print out:
start thread 0
start thread 1
finish thread 1
finish thread 0
*/
int main(int argc, char **argv) {
	pthread_t tid;

	pthread_create(&tid, NULL, thread1, NULL);
	thread0();

	print_tcb_linked_list(runQ_head);
	return 0;
}
