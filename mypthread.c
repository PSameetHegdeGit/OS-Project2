// File:	mypthread.c

// List all group member's name: Shubham Rustagi, Sameet Hedge
// username of iLab: sr1034,
// iLab Server: man

#include "mypthread.h"

tcb *runQ_head = NULL;
tcb *runQ_tail = NULL;

// keep track of if first thread has an associated tcb
int first_thread_init = 0;

// counter for unique thread id
mypthread_t idCounter = 1;


/********************************************************************************************************
                                  					PThreads
*********************************************************************************************************/

/*
 * create a new thread - Shubham
 */
int mypthread_create(mypthread_t *thread, pthread_attr_t *attr, void *(*function)(void*), void *arg) {
	// initialize first thread and queues if not initialized
	if (!first_thread_init) {
		init_queue(&runQ_head, &runQ_tail);
		init_first_thread();
	}

	// create Thread Control Block
	tcb *new_tcb = malloc(sizeof(tcb));

	// initialize ucontext_t struct with current thread context
	if (getcontext( &(new_tcb -> t_context)) == -1) {
		// if getcontext fails
		free(new_tcb);
		return -1;
	}

	// set tcb data
	new_tcb -> t_id = idCounter++;
	new_tcb -> t_status = READY;

	// allocate space of stack for this thread to run
	void *stack = malloc(SIGSTKSZ);

	// set context data for tcb
	// uc_link: context to be executed when the function func returns or it should be a null pointer.
	new_tcb -> t_context.uc_link = NULL;
	new_tcb -> t_context.uc_stack.ss_sp = stack;
    new_tcb -> t_context.uc_stack.ss_size = SIGSTKSZ;
    new_tcb -> t_context.uc_stack.ss_flags = 0;

	// modify ucontext_t struct and start executing function with given arg
	makecontext( &(new_tcb -> t_context), (void(*)(void))function, 1, arg);
	*thread = new_tcb -> t_id;

	// after everything is all set, push this thread control block onto the run queue
	enqueueThread(runQ_head, runQ_tail, new_tcb);
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
                                  			LINKED LIST HELPERS
*********************************************************************************************************/

/*
 * Initialize H and T to point to each other if null head
 */
void init_queue(tcb **headPtr, tcb **tailPtr) {
	if (*headPtr == NULL) {
		*headPtr = malloc(sizeof(tcb));
		*tailPtr = malloc(sizeof(tcb));

		(*headPtr) -> next = *tailPtr;
		(*tailPtr) -> prev = *headPtr;
	}
}

/*
 * Insert a thread control block to the end of given queue
 */
void enqueueThread(tcb *head, tcb* tail, tcb *toInsert) {
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
int dequeueThread(tcb *head, tcb *tail) {
	// make sure list is not initially empty
	if (head -> next == tail) {
		return 0;
	}

	tcb *remove_tcb = head -> next;
	head -> next = remove_tcb -> next;

	// free allocated memory (including stack)
	free(remove_tcb -> t_context.uc_stack.ss_sp);
	free(remove_tcb);

	return 1;
}

/********************************************************************************************************
                                  		THREAD CONTROL BLOCK HELPERS
*********************************************************************************************************/

/*
 * Create the first thread block. This is the thread that creates all other threads
 */
void init_first_thread() {
	tcb *new_tcb = malloc(sizeof(tcb));

	new_tcb -> t_status = RUNNING;
	printf("Making first thread status %d\n", RUNNING);
	new_tcb -> t_id = 0;
	// priority = -1 is the highest priority
	new_tcb -> t_priority = -1;

	enqueueThread(runQ_head, runQ_tail, new_tcb);
	first_thread_init = 1;
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

	if (t_block -> prev == NULL) {
		puts("\n                 ** Head **                      \n");
	} else if (t_block -> next == NULL) {
		puts("\n                 ** Tail **                      \n");
	} else {
		printf(
			"ID: %d\n"
			"Status: %s\n"
			"Priority %d\n",
			t_block -> t_id,
			status[t_block -> t_priority],
			t_block -> t_priority
		);
	}

	puts("---------------------------------------------------");
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
