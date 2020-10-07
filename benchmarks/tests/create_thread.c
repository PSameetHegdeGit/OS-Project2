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
int maxInt = 200000000;

// Run by TID: 1
void* thread0(void *arg) {
	puts("starting thread 0");
	int i;
	while (1) {
		puts("b");
	}
	puts("finished thread 0");
	return NULL;
}

// // Run by TID: 2
// void* thread1(void *arg) {
// 	puts("starting thread 1");
// 	int i;
// 	while(1) {
// 		puts("a")
// 	}
// 	return NULL;
// }

// run by TID: 0
void thread2() {
	puts("starting thread 2");
	int i;
	while(1) {
		puts("a");
	}
	puts("finished thread 2");
}

int main(int argc, char **argv) {
	pthread_t tid[2];

	pthread_create(&tid[0], NULL, thread0, NULL);
	//pthread_create(&tid[1], NULL, thread1, NULL);

	thread2();
	print_tcb_linked_list(runQ_head);
	return 0;
}
