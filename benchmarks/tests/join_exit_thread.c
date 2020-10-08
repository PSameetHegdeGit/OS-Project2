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
	int i;
    int j = 0;
	for (i = 0; i < 100000; i++) {
        j++;
    }
	puts("finished thread 1");
	pthread_exit((void*) -1);
}

// run by TID: 0
void thread0() {
	puts("starting thread 0");
	int i;
    int j = 0;
	for (i = 0; i < 10; i++) {
        j++;
    }
	puts("finished thread 0");
}

// should print out "return value: -1"
int main(int argc, char **argv) {
	pthread_t tid;

	pthread_create(&tid, NULL, thread1, NULL);
	thread0();

	int retVal;
	// thread 0 joining thread 1
    pthread_join(tid, (void**)&retVal);
	print_tcb_linked_list(runQ_head);
	printf("return value: %d\n", retVal);
	return 0;
}
