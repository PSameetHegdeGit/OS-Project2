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

1. Since our signal can come between any two instructions, we want to essentially stop timer and start timer whenever we read/write any data to the queues

2. All threads will use join and exit, so don't need to handle edge cases

3. On exit, just put a TCB in a terminated queue (free it's stack). Then when we join, can free the whole TCB


## Errors

1. Profiling timer error: for whatever reason, sometimes when you run the create_thread.c program under benchmarks/tests, you get an error like "profiling timer expired". Only happens sometimes so could be a race condition, but not really since only one thread runs at any given moment, so not really sure
    - need to stop timer when we get into signal handler
    - need to memset all structs to 0 to fix


