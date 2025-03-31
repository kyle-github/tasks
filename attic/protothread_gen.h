#ifndef PROTO_THREAD_GEN_H
#define PROTO_THREAD_GEN_H

#include "protothread.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// X-macro helper macros
#define PT_ARG_ENTRY(type, name) type name;
#define PT_VAR_ENTRY(type, name) type name;

// Macro to generate argument struct
#define PT_GENERATE_ARG_STRUCT(pt_##name, arg_list) \
    typedef struct { \
        arg_list \
    } pt_##name##_args_t;

// Macro to generate variable struct
#define PT_GENERATE_VAR_STRUCT(pt_##name, var_list) \
    typedef struct { \
        var_list \
    } pt_##name##_vars_t;

// Macro to generate the main protothread struct
#define PT_GENERATE_PT_STRUCT(pt_##name, result_type, arg_list, var_list) \
    typedef struct { \
        pt_t __pt__; \
        result_type result; \
        pt_##name##_args_t args; \
        pt_##name##_vars_t vars; \
    } pt_##name##_pt_t;

// Macro to generate the runner wrapper function declaration
#define PT_GENERATE_RUNNER_WRAPPER(pt_##name) \
    int pt_##name##_runner_wrapper(pt_t *pt);

// Macro to generate the runner function declaration
#define PT_GENERATE_RUNNER(pt_##name, result_type) \
    int pt_##name##_runner(pt_##name##_pt_t *__pt__, pt_##name##_args_t *args, pt_##name##_vars_t *vars, result_type *result);

// Macro to generate the destructor wrapper function declaration
#define PT_GENERATE_DESTRUCTOR_WRAPPER \
    void pt_##name##_destructor_wrapper(pt_t *pt);

// Macro to generate the destructor function declaration
#define PT_GENERATE_DESTRUCTOR_FUNC(pt_##name) \
    void pt_##name##_destructor(pt_##name##_args_t *args, pt_##name##_vars_t *vars, result_type *result);

// Macro to generate the create function
#define PT_GENERATE_CREATE(name, result_type, arg_list) \
    pt_##name##_pt_t *pt_##name##_create( \
        /* Generate argument list from X-macro */ \
        PT_EXPAND_ARG_LIST(arg_list) \
    ) { \
        pt_##name##_pt_t *pt = (pt_##name##_pt_t *)malloc(sizeof(pt_##name##_pt_t)); \
        if (!pt) { \
            return NULL; \
        } \
        memset(pt, 0, sizeof(pt_##name##_pt_t)); \
        pt_init(&(pt->__pt__), pt_##name##_runner_wrapper, pt_##name##_destructor_wrapper); \
        /* Initialize arguments from create function arguments */ \
        PT_INIT_ARGS(pt_##name, arg_list) \
        pt->result = 0; \
        return pt; \
    }

// Macro to expand the argument list for the create function
#define PT_EXPAND_ARG_LIST(arg_list) PT_EXPAND_ARG_LIST_INNER(arg_list)
#define PT_EXPAND_ARG_LIST_INNER(arg_list) PT_EXPAND_ARG_LIST_HELPER arg_list
#define PT_EXPAND_ARG_LIST_HELPER(PT_ARG_ENTRY_MACRO, type, name) type name, PT_EXPAND_ARG_LIST_HELPER_REST
#define PT_EXPAND_ARG_LIST_HELPER_REST(PT_ARG_ENTRY_MACRO, type, name) type name, PT_EXPAND_ARG_LIST_HELPER_REST
// #define PT_EXPAND_ARG_LIST_HELPER_REST(PT_ARG_ENTRY_MACRO, type, name) type name
// #define PT_EXPAND_ARG_LIST_HELPER_REST(PT_ARG_ENTRY_MACRO, type, name) type name

// Macro to initialize arguments in the create function
#define PT_INIT_ARGS(pt_##name, arg_list) PT_INIT_ARGS_INNER(pt_##name, arg_list)
#define PT_INIT_ARGS_INNER(pt_##name, arg_list) PT_INIT_ARGS_HELPER arg_list
#define PT_INIT_ARGS_HELPER(PT_ARG_ENTRY_MACRO, type, name) pt->args.name = name; PT_INIT_ARGS_HELPER_REST
#define PT_INIT_ARGS_HELPER_REST(PT_ARG_ENTRY_MACRO, type, name) pt->args.name = name; PT_INIT_ARGS_HELPER_REST
// #define PT_INIT_ARGS_HELPER_REST(PT_ARG_ENTRY_MACRO, type, name) pt->args.name = name

// Macro to generate all definitions
#define PT_GENERATE_DEFS(pt_##name, result_type, arg_list, var_list) \
    PT_GENERATE_ARG_STRUCT(pt_##name, arg_list) \
    PT_GENERATE_VAR_STRUCT(pt_##name, var_list) \
    PT_GENERATE_PT_STRUCT(pt_##name, result_type, arg_list, var_list) \
    PT_GENERATE_RUNNER_WRAPPER(pt_##name) \
    PT_GENERATE_RUNNER(pt_##name, result_type) \
    PT_GENERATE_DESTRUCTOR_WRAPPER(pt_##name) \
    PT_GENERATE_DESTRUCTOR_FUNC(pt_##name, result_type) \
    PT_GENERATE_CREATE(pt_##name, result_type, arg_list)

// Macro to generate the function header
#define PT_GENERATE_FUNC(pt_##name) \
    int pt_##name##_runner(pt_##name##_pt_t *__pt__, pt_##name##_args_t *args, pt_##name##_vars_t *vars, int *result) { \
        switch(__pt__->__pt__.step) { \
            case PT_START:

// Macro to end the function
#define PT_FUNC_END \
            break; \
            default: \
                __pt__->__pt__.status = PT_ERROR; \
                break; \
        } \
        return 0; \
    } \
    int pt_##name##_runner_wrapper(pt_t *pt) { \
        pt_##name##_pt_t *__pt__ = (pt_##name##_pt_t *)pt; \
        return pt_##name##_runner(__pt__, &__pt__->args, &__pt__->vars, &__pt__->result); \
    }

// Macro to generate the destructor header
#define PT_GENERATE_DESTRUCTOR(pt_##name) \
    void pt_##name##_destructor(pt_##name##_args_t *args, pt_##name##_vars_t *vars, int *result)

#define PT_DESTRUCTOR_END \
    } \
    void pt_##name##_destructor_wrapper(pt_t *pt) { \
        pt_##name##_pt_t *__pt__ = (pt_##name##_pt_t *)pt; \
        pt_##name##_destructor(&__pt__->args, &__pt__->vars, &__pt__->result); \
    }

#endif // PROTO_THREAD_GEN_H
