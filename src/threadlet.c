#include "threadlet.h"
#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 64k should be enough for anyone */
#define STACK_FRAME_SIZE 64 * 1024

typedef struct threadlet {
    jmp_buf context;         /* registers, stack pointer etc. for this threadlet. */
    threadlet_func *t_func;  /* The function that actually gets run. */
    void *data;              /* user/app data pointer */
    intptr_t result;         /* slot for results to be passed back */
    threadlet_state_t state; /* is the threadlet running or dead or ... */
} threadlet;

/* FIXME - wrap this in a real scheduler */
static threadlet *current_threadlet = NULL;
static jmp_buf main_context;

/* This is needed to  */
static void threadlet_trampoline(threadlet *t) {
    if(t && t->t_func != NULL) {
        t->state = THREADLET_RUNNABLE;
        t->state = t->t_func(t->data, &t->result);
    }

    /* Return to the main context after finishing the threadlet */
    if(setjmp(main_context) == 0) { longjmp(main_context, 1); }
}

/* Create a threadlet */


int threadlet_create(threadlet **t, threadlet_func t_func, void *data) {
    if(!t || !t_func) { return -1; }

    *t = malloc(sizeof(threadlet));
    if(!*t) { return -1; }

    /* clear all the memory for the threadlet state */
    memset(*t, 0, sizeof(threadlet));

    (*t)->t_func = t_func;
    (*t)->data = data;
    (*t)->state = THREADLET_RUNNABLE;

    /* Save the context for the threadlet */
    char stack[STACK_FRAME_SIZE];
    (void)stack; /* Suppress unused variable warnings */

    if(setjmp((*t)->context) == 0) { longjmp(main_context, 1); }

    threadlet_trampoline(*t);

    return 0;
}

/* Yield execution */
int threadlet_yield(void) {
    if(!current_threadlet) { return -1; }


    if(setjmp(current_threadlet->context) == 0) { longjmp(main_context, 1); /* Return to the main context */ }

    return 0;
}

/* Resume a threadlet */
int threadlet_resume(threadlet *t) {
    if(!t || t->state != THREADLET_RUNNABLE) { return -1; }

    current_threadlet = t;

    if(setjmp(main_context) == 0) { longjmp(t->context, 1); /* Jump to the threadlet's context */ }


    return 0;
}

/* Join a threadlet */
int threadlet_join(threadlet *t) {
    while(t && t->state != THREADLET_FINISHED && t->state != THREADLET_ERROR) { threadlet_resume(t); }
    if(t) { free(t); }
    return 0;
}
