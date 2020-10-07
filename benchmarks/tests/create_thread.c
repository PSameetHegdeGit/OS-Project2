#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "../../mypthread.h"

/* A scratch program template on which to call and
 * test mypthread library functions as you implement
 * them.
 *
 * You can modify and use this program as much as possible.
 * This will not be graded.
 */


extern tcb *runQ_head;
extern tcb *runQ_tail;

void* thread0(void *arg) {
	int i;
	while (1) {
		i++;
	}
	return NULL;
}

void* thread1(void *arg) {
	int i;
	while(1) {
		i++;
	}
	return NULL;
}

void thread2() {
	int i;
	while(1) {
		i++;
	}
}

int main(int argc, char **argv) {
	pthread_t tid[2];

	puts("creating thread 0...");
	pthread_create(&tid[0], NULL, thread0, NULL);
	puts("creating thread 1...");
	pthread_create(&tid[1], NULL, thread1, NULL);

	thread2();
	print_tcb_linked_list(runQ_head);
	return 0;
}
