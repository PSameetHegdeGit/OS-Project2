// File:	mypthread_t.h

// List all group member's name: Shubham Rustagi, Sameet Hedge
// username of iLab: sr1034,
// iLab Server: man

#ifndef MYTHREAD_T_H
#define MYTHREAD_T_H

#define _GNU_SOURCE

/* To use Linux pthread Library in Benchmark, you have to comment the USE_MYTHREAD macro */
#define USE_MYTHREAD 1

/* include lib header files that you need here: */
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <signal.h>

typedef uint mypthread_t;
typedef enum status{READY, RUNNING, WAITING, TERMINATED} status;

typedef struct threadControlBlock {
	// thread Id
	mypthread_t t_id;

	// thread status
	status t_status;

	// thread context
	ucontext_t t_context;

	// thread stack (don't think we need since it's captured by t_context)
	// void* t_stack;

	// thread priority
	int t_priority;

	// pointer to return value of function run by thread
    void *t_return_val;

	// doubly linked list pointers
	struct threadControlBlock *next;
	struct threadControlBlock *prev;
} tcb;

/* mutex struct definition */
typedef struct mypthread_mutex_t {
	/* add something here */
} mypthread_mutex_t;


/* define your data structures here: */
// Feel free to add your own auxiliary data structures (linked list or queue etc...)


int mypthread_create(mypthread_t * thread, pthread_attr_t * attr, void *(*function)(void*), void * arg);
int mypthread_yield();
void mypthread_exit(void *value_ptr);
int mypthread_join(mypthread_t thread, void **value_ptr);

int mypthread_mutex_init(mypthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr);
int mypthread_mutex_lock(mypthread_mutex_t *mutex);
int mypthread_mutex_unlock(mypthread_mutex_t *mutex);
int mypthread_mutex_destroy(mypthread_mutex_t *mutex);

void init_queue(tcb **headPtr, tcb **tailPtr);
void enqueueThread(tcb *head, tcb* tail, tcb *toInsert);
int dequeueThread(tcb *head, tcb *tail);

void init_first_thread();

void print_tcb(tcb* t_block);
void print_tcb_linked_list(tcb* head);

#ifdef USE_MYTHREAD
#define pthread_t mypthread_t
#define pthread_mutex_t mypthread_mutex_t
#define pthread_create mypthread_create
#define pthread_exit mypthread_exit
#define pthread_join mypthread_join
#define pthread_mutex_init mypthread_mutex_init
#define pthread_mutex_lock mypthread_mutex_lock
#define pthread_mutex_unlock mypthread_mutex_unlock
#define pthread_mutex_destroy mypthread_mutex_destroy
#endif

#endif
