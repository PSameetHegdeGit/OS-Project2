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

## TODO

1. Check benchmarks


## Notes

1. All threads will use join and exit, so don't need to handle edge cases


## Errors

1. None as of now :)

