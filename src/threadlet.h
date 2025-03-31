
#pragma once

#include <stdint.h>

typedef enum {
    THREADLET_RUNNABLE,
    THREADLET_FINISHED,
    THREADLET_ERROR
} threadlet_state_t;

typedef struct threadlet threadlet;
typedef threadlet_state_t (threadlet_func)(void *data, intptr_t *result);

extern int threadlet_create(threadlet **t, threadlet_func *func, void *data);
extern int threadlet_resume(threadlet *t);
extern int threadlet_yield(void);
extern int threadlet_join(threadlet *t);
