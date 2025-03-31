#ifndef PROTO_THREAD_H
#define PROTO_THREAD_H

#include <stddef.h> // For size_t
#include <stdint.h> // For intptr_t

// Protothread status codes
typedef enum {
    PT_INIT = 0,
    PT_RUNNING,
    PT_DONE,
    PT_ERROR,
    PT_YIELDED,
    PT_START
} pt_status_t;

// Generic protothread structure
typedef struct pt_t {
    int step; // Used in protothread switch/case implementation
    pt_status_t status; // Status of protothread

    int (*pt_runner_wrapper)(struct pt_t *pt);
    void (*pt_destructor_wrapper)(struct pt_t *pt);
    void *result;
    void *args;
    void *vars;
} pt_t;

// Protothread initialization
#define pt_init(pt, runner, destructor) do { \
    (pt)->step = PT_START; \
    (pt)->status = PT_RUNNING; \
    (pt)->pt_runner_wrapper = (runner); \
    (pt)->pt_destructor_wrapper = (destructor); \
} while(0)

// Protothread yield
#define pt_yield(val) do { \
    *result = val; \
    __pt__->step = __LINE__; \
    __pt__->status = PT_YIELDED; \
    return 0; \
    case __LINE__:; \
} while(0)

// Protothread exit
#define pt_exit(status) do { \
    __pt__->status = status; \
    return 0; \
} while(0)

// Protothread resume
#define pt_resume(pt) ((pt)->__pt__.pt_runner_wrapper(&((pt)->__pt__)))

// Protothread status
#define pt_status(pt) ((pt)->__pt__.status)

// Protothread result
#define pt_result(pt) (*((typeof((pt)->result)*)(pt)->result))

// Protothread join (destructor)
#define pt_join(pt) do { \
    if((pt)->__pt__.pt_destructor_wrapper) { \
        (pt)->__pt__.pt_destructor_wrapper(&((pt)->__pt__)); \
    } \
    free(pt); \
} while(0)

#endif // PROTO_THREAD_H
