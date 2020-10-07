#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "../../mypthread.h"

/*
 * Prints out:
 * A
 * A
 * ...
 * B
 * B
 * ...
 * A
 * A
 * ....
 */

extern tcb *runQ_head;
extern tcb *runQ_tail;
int maxInt = 200000000;

// Run by TID: 1
void* thread1(void *arg) {
	puts("starting thread 1");
	int i;
	while (1) {
		puts("b");
	}
	puts("finished thread 1");
	return NULL;
}

// run by TID: 0
void thread0() {
	puts("starting thread 0");
	int i;
	while(1) {
		puts("a");
	}
	puts("finished thread 0");
}


int main(int argc, char **argv) {
	pthread_t tid[1];

	pthread_create(&tid[1], NULL, thread1, NULL);
	//pthread_create(&tid[1], NULL, thread1, NULL);

	thread0();
	print_tcb_linked_list(runQ_head);
	return 0;
}
