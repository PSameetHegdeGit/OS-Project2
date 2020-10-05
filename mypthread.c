// File:	mypthread.c

// List all group member's name: Shubham Rustagi, Sameet Hedge
// username of iLab: sr1034,
// iLab Server: man

#include "mypthread.h"

tcb *runQ_head = NULL;
tcb *runQ_tail = NULL;

/********************************************************************************************************
                                  					PThreads
*********************************************************************************************************/

/*
 * create a new thread - Shubham
 */
int mypthread_create(mypthread_t * thread, pthread_attr_t * attr, void *(*function)(void*), void * arg) {
       // create Thread Control Block
       // create and initialize the context of this thread
       // allocate space of stack for this thread to run
       // after everything is all set, push this thread int
    return 0;
};

/*
 * give CPU possession to other user-level threads voluntarily
 */
int mypthread_yield() {
	// change thread state from Running to Ready
	// save context of this thread to its thread control block
	// wwitch from thread context to scheduler context
	return 0;
};

/*
 * terminate a thread
 */
void mypthread_exit(void *value_ptr) {
	// Deallocated any dynamic memory created when starting this thread
};


/*
 * Wait for thread termination
 */
int mypthread_join(mypthread_t thread, void **value_ptr) {
	// wait for a specific thread to terminate
	// de-allocate any dynamic memory created by the joining thread
	return 0;
};

/********************************************************************************************************
                                  					Mutex
*********************************************************************************************************/

/*
 * initialize the mutex lock
 */
int mypthread_mutex_init(mypthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr) {
	//initialize data structures for this mutex
	return 0;
};

/*
 *  aquire the mutex lock
 */
int mypthread_mutex_lock(mypthread_mutex_t *mutex) {
        // use the built-in test-and-set atomic function to test the mutex
        // if the mutex is acquired successfully, enter the critical section
        // if acquiring mutex fails, push current thread into block list and //
        // context switch to the scheduler thread
        return 0;
};

/*
 * release the mutex lock
 */
int mypthread_mutex_unlock(mypthread_mutex_t *mutex) {
	// Release mutex and make it available again.
	// Put threads in block list to run queue
	// so that they could compete for mutex later.
	return 0;
};

/*
 * destroy the mutex
 */
int mypthread_mutex_destroy(mypthread_mutex_t *mutex) {
	// Deallocate dynamic memory created in mypthread_mutex_init
	return 0;
};

/********************************************************************************************************
                                  					Scheduler
*********************************************************************************************************/

/*
 * scheduler
 */
static void schedule() {
	// Every time when timer interrupt happens, your thread library
	// should be contexted switched from thread context to this
	// schedule function

	// Invoke different actual scheduling algorithms
	// according to policy (STCF or MLFQ)

	// if (sched == STCF)
	//		sched_stcf();
	// else if (sched == MLFQ)
	// 		sched_mlfq();

// schedule policy
#ifndef MLFQ
	// Choose STCF
#else
	// Choose MLFQ
#endif

}

/*
 * Preemptive SJF (STCF) scheduling algorithm
 */
static void sched_stcf() {
	// Your own implementation of STCF
	// (feel free to modify arguments and return types)
}

/*
 * Preemptive MLFQ scheduling algorithm
 */
static void sched_mlfq() {
	// Your own implementation of MLFQ
	// (feel free to modify arguments and return types)
}

/********************************************************************************************************
                                  				GENERAL HELPERS
*********************************************************************************************************/

/*
 * Insert a thread control block to the end of given queue
 */
void enqueue(tcb *head, tcb* tail, tcb *toInsert) {
	// initialize H and T to point to each other if null head
	if (head == NULL) {
		head -> next = tail;
		tail -> prev = head;
	}

	// insert node at end of queue
	tcb *currLast = tail -> prev;
	currLast -> next = toInsert;

	toInsert -> prev = currLast;
	toInsert -> next = tail;
	tail -> prev = toInsert;
}

/*
 * Remove a thread control block from start of given queue.
 *
 * Return 1 on successful deletion, 0 otherwise (including empty list)
 */
int dequeue(tcb *head, tcb *tail) {
	// make sure list is not initially empty
	if (head -> next == tail) {
		return 0;
	}

	head -> next = (head -> next) -> next;
	return 1;
}

/********************************************************************************************************
                                  				SIDE HELPERS
*********************************************************************************************************/

/*
 * Print details of given thread block
 */
void print_tcb(tcb* t_block) {
	char* status[] = {"ready", "running", "waiting", "terminated"};

	puts("\n--------------Thread control block-----------------");
	printf(
		"ID: %d\n"
		"Status: %d\n"
		"Priority %s\n",
		t_block -> t_id,
		t_block -> t_status,
		status[t_block -> t_priority]
	);
	puts("---------------------------------------------------\n");
}

/*
 * Print details of given linked list of thread control blocks
 */
void print_tcb_linked_list(tcb* head) {
	while(head) {
		print_tcb(head);
		head = head -> next;
	}
}
