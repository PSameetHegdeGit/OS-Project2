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

// Run by TID: 1
void* thread1(void *arg) {
	puts("starting thread 1");
	int i = 0;
	while (1) {
		if (i % 10000) {
			puts("b");
		}
		i++;
	}
}

// run by TID: 0
void thread0() {
	puts("starting thread 0");
	int i = 0;
	while(1) {
		if (i % 10000) {
			puts("a");
		}
		i++;
	}
}


int main() {
	pthread_t tid;
	pthread_create(&tid, NULL, thread1, NULL);

	thread0();
	print_tcb_linked_list(runQ_head);
	return 0;
}
