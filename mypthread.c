// File:	mypthread.c

// List all group member's name: Shubham Rustagi, Sameet Hedge
// username of iLab: sr1034,
// iLab Server: man

#include "mypthread.h"

/*
 * Doubly linked lists for keeping track of:
 * 1. Running/ready threads
 * 2. Terminated threads
 *
 * General structure: Head <-> P0 <-> P1 <-> P2 <-> Tail
 * P0 (head -> next) will always be the running process for the runQ
 * Note: head and tail are meaningless thread blocks themselves
 */

tcb *runQ_head = NULL;
tcb *runQ_tail = NULL;

tcb *termQ_head = NULL;
tcb *termQ_tail = NULL;

// keep track of whether library (queues, timer, etc..) has been initialized
int init_lib = 0;

// counter for unique thread id
mypthread_t idCounter = 1;


/********************************************************************************************************
                                  					PThreads
*********************************************************************************************************/

/*
 * create a new thread
 */
int mypthread_create(mypthread_t *thread, pthread_attr_t *attr, void *(*function)(void*), void *arg) {
	stopTimer();

	// initialize queues, add current first thread, and start timer if not initialized
	if (!init_lib) {
		init_queue(&runQ_head, &runQ_tail);
		init_queue(&termQ_head, &termQ_tail);

		init_first_thread();
		register_sigprof_handler();

		init_lib = 1;
	}

	// create Thread Control Block
	tcb *new_tcb = calloc(1, sizeof(tcb));

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
	enqueue_tcb(runQ_head, runQ_tail, new_tcb);

	startTimer();
	return 0;
};

/*
 * give CPU possession to other user-level threads voluntarily
 */
int mypthread_yield() {
	tcb *curr_running = runQ_head -> next;

	// change thread state from Running to Ready
	curr_running -> t_status = READY;

	// save context of this thread to its thread control block
	save_running_context_to_tcb();

	// switch from thread context to scheduler context
	schedule();
	return 0;
}

/*
 * terminate a thread
 */
void mypthread_exit(void *value_ptr) {
	tcb *curr_running = runQ_head -> next;

	// Deallocated any dynamic memory created when starting this thread (done in join)

	// change thread state from Running to Terminated
	curr_running -> t_status = TERMINATED;

	// assign return value pointer
    curr_running -> t_return_val = value_ptr;

	// save context of this thread to its thread control block
	save_running_context_to_tcb();
	schedule();
}


/*
 * Wait for thread termination
 */
int mypthread_join(mypthread_t thread, void **value_ptr) {

	// look through the run queue to see if the tcb is there
	tcb* join_target = find_tcb_by_id(runQ_head, runQ_tail, thread);

	// found in run queue, so we can just yield current thread and wait for
	// that thread to finish it's execution
	if (join_target) {
		while(join_target -> t_status != TERMINATED) {
			mypthread_yield();
   		}
	}
	// otherwise, look through the terminated queue to see if the tcb is there
	else {
		tcb* join_target = find_tcb_by_id(termQ_head, termQ_tail, thread);
	}

	// not found in run queue or terminated queue so tcb doesn't exist
	if (join_target == NULL) {
		return -1;
	}

	if (value_ptr) {
		*value_ptr = join_target -> t_return_val;
	}

	// de-allocate any dynamic memory created by the joining thread
	remove_tcb(join_target, 1);
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
 * scheduler - invoked on timer interrupt or thread yields (decide what to run next)
 */
static void schedule() {
	stopTimer();
	tcb *initialFirst = runQ_head -> next;

	// increment the quantum of time elapsed of current thread
	initialFirst -> t_quantum_elapsed += 1;

	// after exit, remove terminated tcb from runqueue and put in terminated queue, if it exists
	remove_terminated();

	// after the last thread calls exit, nothing left in run queue, so simply exit process
	if (runQ_head -> next == runQ_tail) {
		free_queue(runQ_head);
		free_queue(termQ_head);
		exit(EXIT_SUCCESS);
	}

	// schedule policy
	#ifndef MLFQ
		// Choose STCF
		sched_stcf();
	#else
		// Choose MLFQ
		sched_mlfq();
	#endif

	tcb *curr_first = runQ_head -> next;
	startTimer();

	// save the current context and set the context to the new running process context
	// printf("swapping context to %d\n", curr_first -> t_id);
	swapcontext( &(initialFirst -> t_context), &(curr_first -> t_context));
}

/*
 * Preemptive SJF (STCF) scheduling algorithm.
 * Find the tcb that has the smallest t_quantum_elapsed and make that the running process
 */
static void sched_stcf() {
	// (feel free to modify arguments and return types)
	int minQuantum = INT_MAX;
	tcb *min_t_block = NULL;

	tcb *curr_tcb = runQ_head -> next;
	// traverse run queue and find tcb with smallest quantum elapsed
	while(curr_tcb != runQ_tail) {
		if (curr_tcb -> t_quantum_elapsed < minQuantum) {
			minQuantum = curr_tcb -> t_quantum_elapsed;
			min_t_block = curr_tcb;
		}
		curr_tcb = curr_tcb -> next;
	}

	if (min_t_block) {
		// remove the tcb from the run queue and enqueue it back at position 1
		remove_tcb(min_t_block, 0);
		enqueue_tcb_first(runQ_head, runQ_tail, min_t_block);

		// change status from Ready to Running
		min_t_block -> t_status = RUNNING;
	}
}

/*
 * Preemptive MLFQ scheduling algorithm
 *
 * TODO: confirm this is only for 518
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
		*headPtr = calloc(1, sizeof(tcb));
		*tailPtr = calloc(1, sizeof(tcb));

		(*headPtr) -> next = *tailPtr;
		(*tailPtr) -> prev = *headPtr;
	}
}

/*
 * Insert a thread control block to the start of given queue
 * This is essentially making *toInsert the running process
 */
void enqueue_tcb_first(tcb *head, tcb* tail, tcb *toInsert) {
	if (toInsert) {
		tcb *currFirst = head -> next;
		head -> next = toInsert;

		toInsert -> next = currFirst;
		toInsert -> prev = head;
		currFirst -> prev = toInsert;
	}
}

/*
 * Insert a thread control block to the end of given queue
 */
void enqueue_tcb(tcb *head, tcb* tail, tcb *toInsert) {
	if (toInsert) {
		tcb *currLast = tail -> prev;
		currLast -> next = toInsert;

		toInsert -> prev = currLast;
		toInsert -> next = tail;
		tail -> prev = toInsert;
	}
}

/*
 * Remove a thread control block from start of given queue.
 *
 * Return NULL if empty list or if freeing memory used by tcb,
 * else returns a ptr to tcb
 */
tcb* dequeue_tcb(tcb *head, tcb *tail, int freeMemory) {
	// make sure list is not initially empty
	if (head -> next == NULL || head -> next == tail) {
		return NULL;
	}

	// remove first node
	tcb *remove_tcb = head -> next;
	tcb *after_remove_tcb = remove_tcb -> next;

	head -> next = remove_tcb -> next;
	after_remove_tcb -> prev = head;

	// free allocated memory (including stack)
	if (freeMemory) {
		free_tcb(remove_tcb);
		return NULL;
	}

	return remove_tcb;
}

/*
 * Remove a thread control block at given pointer from queue.
 */
void remove_tcb(tcb *t_block, int freeMemory) {
	if (t_block) {
		tcb *prev_t_block = t_block -> prev;
		tcb *next_t_block = t_block -> next;


		prev_t_block -> next = next_t_block;
		next_t_block -> prev = prev_t_block;
	}

	// free allocated memory (including stack)
	if (freeMemory) {
		free_tcb(t_block);
	}
}

/*
 * Removes any terminated process from run queue to terminated queue
 */
void remove_terminated() {
	tcb *currRunning = runQ_head -> next;

	// if we just exited, remove from run queue and put at the end of the terminated queue
	if (currRunning -> t_status == TERMINATED) {
		dequeue_tcb(runQ_head, runQ_tail, 0);
		enqueue_tcb(termQ_head, termQ_tail, currRunning);
	}
}

/*
 * Find a thread control block by it's given thread id in given queue
 * Return a pointer to the block or NULL if not found
 */
tcb* find_tcb_by_id(tcb *head, tcb *tail, mypthread_t thread_id) {
	head = head -> next;
	while (head != tail) {
		if (head -> t_id == thread_id) {
			return head;
		}
		head = head -> next;
	}
	return NULL;
}

/********************************************************************************************************
                                  		THREAD CONTROL BLOCK HELPERS
*********************************************************************************************************/

/*
 * Create the first thread block. This is the thread that creates all other threads
 */
void init_first_thread() {
	tcb *new_tcb = calloc(1, sizeof(tcb));

	new_tcb -> t_status = RUNNING;
	new_tcb -> t_id = idCounter++;
	// priority = -1 is the highest priority
	new_tcb -> t_priority = -1;

	enqueue_tcb(runQ_head, runQ_tail, new_tcb);
}

/*
 * Free all allocated memory of a thread control block
 */
void free_tcb(tcb *t_block) {
	free(t_block -> t_context.uc_stack.ss_sp);
	free(t_block);
}

/*
 * Free all allocated memory of a queue (including head and tail tcb)
 */
void free_queue(tcb *head) {
	while (head) {
		tcb *temp = head;
		head = head -> next;
		free_tcb(temp);
	}
}

/*
 * Save context of current running thread in it's respective current thread block
 */
void save_running_context_to_tcb() {
	// make sure runQ at least has one element inside
	if (runQ_head -> next != runQ_tail) {
		tcb *curr_running = runQ_head -> next;
		getcontext( &(curr_running -> t_context));
	}
}

/********************************************************************************************************
                                  				TIMER HELPERS
*********************************************************************************************************/

/*
 * Initialize timer to run and create a timer interrupt every quantum
 * Register signal handler to handle SIGPROF signal
 *
 * Source:
 * https://www.cs.princeton.edu/courses/archive/spring04/cos217/precepthandouts/19/testitimersigaction.c
 */
void register_sigprof_handler() {
	struct sigaction sigAction;
	memset(&sigAction, 0, sizeof(struct sigaction));

	// register a signal handler to handle SIGPROF signals
	sigAction.sa_handler = handleSigProf;
	sigemptyset(&sigAction.sa_mask);
	sigaction(SIGPROF, &sigAction, NULL);
}

/*
 * Respond to the SIGPROF timer interrupt by changing thread state and going to the scheduler code
 */
void handleSigProf(int num) {
	stopTimer();

	// change thread state from Running to Ready
	tcb *curr_running = runQ_head -> next;
	curr_running -> t_status = READY;

	// save context of this thread to its thread control block
	save_running_context_to_tcb();
	schedule();
}

/*
 * Timer decrements from it_value to 0, issues a SIGPROF signal, and resets to it_interval
 */
void startTimer() {
	struct itimerval timerValue;

	timerValue.it_value.tv_sec = 0;
	timerValue.it_value.tv_usec = 1;
	timerValue.it_interval.tv_sec = 0;
	timerValue.it_interval.tv_usec = QUANTUM;
	setitimer(ITIMER_PROF, &timerValue, NULL);
}

/*
 * If both fields in new_value.it_value are zero, then the timer is disarmed.
 */
void stopTimer() {
	struct itimerval timerValue;

	timerValue.it_value.tv_sec = 0;
	timerValue.it_value.tv_usec = 0;
	timerValue.it_interval.tv_sec = 0;
	timerValue.it_interval.tv_usec = 0;
	setitimer(ITIMER_PROF, &timerValue, NULL);
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
			"Priority: %d\n"
			"Quantum elapsed: %d\n"
			"Next: %d\n"
			"Prev: %d\n",
			t_block -> t_id,
			status[t_block -> t_status],
			t_block -> t_priority,
			t_block -> t_quantum_elapsed,
			(t_block -> next) -> t_id,
			(t_block -> prev) -> t_id
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
