# OS-Project2
Project 2 for Operating Systems: Working further with Threads

## General Struct Info
### Context structs:
https://man7.org/linux/man-pages/man3/getcontext.3.html

ucontext_t type is a structure that has at least the following fields:

```
typedef struct ucontext_t {
    struct ucontext_t *uc_link;
    sigset_t          uc_sigmask;
    stack_t           uc_stack;
    mcontext_t        uc_mcontext;
    ...
} ucontext_t;
```

uc_link points to the context that will be resumed when the current context terminates (in case the current context was created using makecontext()), uc_stack is the stack used by this context

### Timer structs:
```
struct itimerval {
    struct timeval it_interval; // next value
    struct timeval it_value;    //
};
```
```
struct timeval {
    time_t      tv_sec;         //
    suseconds_t tv_usec;        //
};
```

## Testing
1. Go to /benchmarks/tests and create a source file and include it in the makefile in the same directory
2. Have 2 shells open, one in benchmarks/tests and the other in the base dir
3. From the shell in the base dir:
```
make clean
make
```
4. From the shell in benchmarks/tests:
```
make clean
make
./{executable}
```

## TODO for Makefile

1. Should add various targets in makefile like:
```
make create
make join
```

2. Should just add the target of the base dir make into the Makefile in benchmarks/tests so we don't have to keep opening 2 shells and having to run Number 4 in testing


## TODO

1. Since our signal can come between any two instructions, we want to essentially freeze (or lock) our timer when we're writing data to our linked lists so that memory doesn't get corrupted. Then, after we finish writing, we can continue. Basically we want to ensure that the write gets completed.

2. Should we only send the signal to the current running thread? I think the signal is recived by all threads if I'm not mistaken. Might want to double check and handle accordingly (guess this doesn't matter since one thread only runs at a given time. what's the point of user level libraries then if they can't do anything in parallel?)

3. So when the first time you call pthread_create, you actually generate 2 threads, 1 is the original and another is what you just created. If the original's (parent) code finishes first and it doesn't join, the process terminates. We don't really handle that I believe so not really sure how to check the "if parent code is done" condition to terminate the process. Is this how it's supposed to go? Are all threads supposed to call pthread_exit after they finish - if so, how?

4. Kind of going of 3, how does our code handle the 1 process left part? Might not have caught all the edge cases. Not really sure how to best test and see results

5. Exit from process if there are no threads in run queue?
    - how do you know when a thread is done? We want to switch to scheduler function, but all we really have control over is uc_link and that requires us to make schedule into a context, not a function.

6. If I have thread0 that created thread1 and thread1 finishes execution in 1 second and timer interrupt happens every 2 second, what will be the result after thread1 finishes execution? Does it just hang for another second and we go scheduler? How do we know it's done and mark it as completed and remove it from the run queue? Will it just be wasting clock cycles if scheduled to run again since it's still in run queue but now has no more code to execute?
    - Actually just terminates and we never go back to thread 0

7. If thread0 creates thread1. What is supposed to happen in each, assuming a 2 second timer interval:
    - thread0 takes 1 sec to complete and thread1 takes 1 minute
    - thread1 takes 1 minute to complete and thread0 takes 1 minute

8. Profiling timer error
    - are you allowed to use swapcontext and whatever is in scheduler from the signal handler? signal safe?

9. If there's just one thread and it yields, what should happen? If no thread has been created and yield is called, what happens?

10. When to clean up all memory? For example, if a thread exits and it doesn't have join called on it, and the main thread finishes execution, how do we just clean everything up? Is there some way for us to test "when all code is done running"

11. What happens if you try to join a nonexisting thread? Is returning -1 fine?


## Errors

1. For whatever reason, sometimes when you run the create_thread.c program under benchmarks/tests, you get an error like "profiling timer expired". Only happens sometimes so could be a race condition, but not really since only one thread runs at any given moment, so not really sure
    - The signal handler is somehow getting reset (https://stackoverflow.com/questions/39501942/signal-handling-behavior-is-reset-to-default-with-sigaction) to its default behavior. Might be because getconext() is unsafe? idk


