#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "../../mypthread.h"

/*
 * Prints out:

 */

extern tcb *runQ_head;
extern tcb *runQ_tail;

// Run by TID: 2
void* thread2(void *arg) {
	puts("starting thread 1");
	int i = 0;
	while (1) {
		if (i % 10000) {
			puts("b");
		}
		i++;
	}
}

// run by TID: 1
void thread1() {
	puts("starting thread 0");
	int i = 0;
	while(1) {
		if (i % 10000) {
			puts("a");
		}
		i++;
	}
}

/*
 * Test pthread create (infinite loop)
 * should print out:
 * 		A
 * 		A
 * 		...
 * 		B
 * 		B
 * 		...
 * 		A
 * 		A
 * 		....
 */
int main() {
	pthread_t tid;
	pthread_create(&tid, NULL, thread2, NULL);

	thread1();
	print_tcb_linked_list(runQ_head);
	return 0;
}
