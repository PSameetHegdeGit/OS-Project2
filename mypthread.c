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
 * create a new thread - Shubham
 */
int mypthread_create(mypthread_t *thread, pthread_attr_t *attr, void *(*function)(void*), void *arg) {
	// initialize queues, add current first thread, and start timer if not initialized
	if (!init_lib) {
		init_queue(&runQ_head, &runQ_tail);
		init_queue(&termQ_head, &termQ_tail);

		init_first_thread();
		init_timer(1);

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
	enqueueThread(runQ_head, runQ_tail, new_tcb);

	// TODO: switch context to created thread
	return 0;
};

/*
 * give CPU possession to other user-level threads voluntarily
 *
 * TODO: edge case:
 * 1. what happens when there's just one thread?
 * Does it just keep executing after yielding?
 * 2. what happens when there are no threads?
 * We call mypthread_yield without creating?
 *
 * 3. Timer
 *
 */
int mypthread_yield() {
	tcb *curr_running = runQ_head -> next;

	// change thread state from Running to Ready
	curr_running -> t_status = READY;

	// save context of this thread to its thread control block
	save_running_context_to_tcb();

	// switch from thread context to scheduler context - done in schedule()
	schedule();
	return 0;
}

/*
 * terminate a thread
 * TODO: Need to go back to scheduler?
 * TODO: What happens when you exit but it hasn't had join called on it?
 */
void mypthread_exit(void *value_ptr) {
	tcb *curr_running = runQ_head -> next;

	// Deallocated any dynamic memory created when starting this thread
	free(curr_running -> t_context.uc_stack.ss_sp);

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
 * "context switch" to here when timer interrupt happens or thread yields (need to decide what to run next)
 *
 * TODO: confirm whether we acutally need a context switch
 */
static void schedule() {
	// Every time when timer interrupt happens, your thread library
	// should be contexted switched from thread context to this
	// schedule function

	// TODO: stop timer
	stopTimer();

	tcb *initialFirst = runQ_head -> next;

	// after threads have called functions like exit, yield, we need to clean up our queues
	prepareQueues();

	// change first element of the run queue since that is what will be run


	// for now, just make whatever the next process is to running
	tcb *curr_first = runQ_head -> next;
	curr_first -> t_status = RUNNING;

	// TODO: start timer
	startTimer();

	// save the current context and set the context to the new running process context
	printf("swapping context to %d\n", curr_first -> t_id);
	swapcontext( &(initialFirst -> t_context), &(curr_first -> t_context));

	// // schedule policy
	// #ifndef MLFQ
	// 	// Choose STCF
	// 	sched_stcf();
	// #else
	// 	// Choose MLFQ
	// 	sched_mlfq();
	// #endif
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
void enqueueThreadFirst(tcb *head, tcb* tail, tcb *toInsert) {
	tcb *currFirst = head -> next;
	head -> next = toInsert;

	toInsert -> next = currFirst;
	toInsert -> prev = head;
	currFirst -> prev = toInsert;
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
 * Return NULL if empty list or if freeing memory used by tcb,
 * else returns a ptr to tcb
 */
tcb* dequeueThread(tcb *head, tcb *tail, int freeMemory) {
	// make sure list is not initially empty
	if (head -> next == NULL || head -> next == tail) {
		return NULL;
	}

	tcb *remove_tcb = head -> next;
	head -> next = remove_tcb -> next;

	// free allocated memory (including stack)
	if (freeMemory) {
		free_tcb(remove_tcb);
		return NULL;
	}

	return remove_tcb;
}

/*
 * Called by scheduler to clean up our queues after functions like yield and exit.
 * Dequeue the current process and enqueue it into the appropriate queue
 */
void prepareQueues() {
	tcb *currRunning = runQ_head -> next;

	// if we just had timer intterupt or yielded
	if (currRunning -> t_status == READY) {
		// put current thread block at the end of the runqueue
		dequeueThread(runQ_head, runQ_tail, 0);
		enqueueThread(runQ_head, runQ_tail, currRunning);
	}
	// if we just exited
	else if (currRunning -> t_status == TERMINATED) {
		// put current thread at the end of the terminated queue
		dequeueThread(runQ_head, runQ_tail, 0);
		enqueueThread(termQ_head, termQ_tail, currRunning);
	}
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
	new_tcb -> t_id = 0;
	// priority = -1 is the highest priority
	new_tcb -> t_priority = -1;

	enqueueThread(runQ_head, runQ_tail, new_tcb);
}

/*
 * Free all allocated memory of a thread control block
 */
void free_tcb(tcb *t_block) {
	free(t_block -> t_context.uc_stack.ss_sp);
	free(t_block);
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
struct itimerval {
    struct timeval it_interval;
    struct timeval it_value;
};

struct timeval {
    time_t      tv_sec;
    suseconds_t tv_usec;
};
*/

/*
 * Initialize timer to run and create a timer interrupt every quantum
 * Register signal handler to handle SIGPROF signal
 *
 * Source:
 * https://www.cs.princeton.edu/courses/archive/spring04/cos217/precepthandouts/19/testitimersigaction.c
 */
void init_timer(int registerSignalHandler) {
	struct sigaction sigAction;

	// register a signal handler to handle SIGPROF signals only once
	if (registerSignalHandler) {
		sigAction.sa_handler = handleSigProf;
		sigemptyset(&sigAction.sa_mask);
		sigaction(SIGPROF, &sigAction, NULL);
	}
	startTimer();
}

/*
 * Respond to the SIGPROF timer interrupt by changing thread state and going to the scheduler code
 */
void handleSigProf(int num) {
	write(STDOUT_FILENO, "handler for timer\n", 19);
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
	write(STDOUT_FILENO, "starting timer\n", 16);
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
	write(STDOUT_FILENO, "stopping timer\n", 16);
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
			"Priority %d\n",
			t_block -> t_id,
			status[t_block -> t_status],
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
