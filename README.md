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

2. Should we only send the signal to the current running thread? I think the signal is recived by all threads if I'm not mistaken. Might want to double check and handle accordingly

3. So when the first time you call pthread_create, you actually generate 2 threads, 1 is the original and another is what you just created. If the original's (parent) code finishes first and it doesn't join, the process terminates. We don't really handle that I believe so not really sure how to check the "if parent code is done" condition to terminate the process

4. Kind of going of 3, how does our code handle the 1 process left part? Might not have caught all the edge cases. Not really sure how to best test and see results
