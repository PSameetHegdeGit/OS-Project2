# OS-Project2
Project 2 for Operating Systems: Working further with Threads

## General Info
https://man7.org/linux/man-pages/man3/getcontext.3.html

ucontext_t type is a structure that has at least the following fields:

typedef struct ucontext_t {
    struct ucontext_t *uc_link;
    sigset_t          uc_sigmask;
    stack_t           uc_stack;
    mcontext_t        uc_mcontext;
    ...
} ucontext_t;

uc_link points to the context that will be resumed when the current context terminates
(in case the current context was created using makecontext()), uc_stack is the stack
used by this context (see sigaltstack(2))
