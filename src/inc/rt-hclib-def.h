/*
 * This file is subject to the license agreement located in the file LICENSE
 * and cannot be distributed without it. This notice cannot be
 * removed or modified.
 *
 * contact: https://github.com/vcave
 */

#ifndef HCLIB_DEF_H_
#define HCLIB_DEF_H_


/**
 * This file contains runtime-level HCLIB data structures
 */

#include "hclib.h"

#define CHECKED_EXECUTION 0

/**
 * @brief An async definition
 */
typedef struct {
    void * fct_ptr;
    void * arg;
    struct ddf_st ** ddf_list; // Null terminated list
    struct _phased_t * phased_clause;
} async_t;

typedef struct finish {
    volatile int counter;
#if CHECKED_EXECUTION
    int owner; //TODO correctness tracking
#endif
    struct finish * parent;
    struct _accum_t ** accumulators; //TODO generify that ?
} finish_t;

struct _async_task_t;
struct _forasync_task_t;

/**
 * @brief Function pointer to an async executor
 */
typedef void (*asyncExecutorFct_t) (struct _async_task_t * async_task);
typedef void (*forasyncExecutorFct_t) (struct _forasync_task_t *forasync_task);

// Fwd declaration for phaser context
struct _phaser_context_t;

/**
 * @brief The HCLIB view of an async task
 * @param def contains data filled in by the user (args, await list, etc.)
 */
typedef struct _async_task_t {
    finish_t * current_finish;
    async_t def;
    #ifdef HAVE_PHASER
    struct _phaser_context_t * phaser_context;
    #endif
} async_task_t;

typedef struct {
    async_t * user;
} forasync_t;

typedef struct _forasync_task_t {
    async_task_t forasync_task;
} forasync_task_t;

typedef struct {
    forasync_t base;
    loop_domain_t loop0;
} forasync1D_t;

typedef struct _forasync_1D_task_t {
    forasync_task_t task;
    forasync1D_t def;
} forasync1D_task_t;

typedef struct {
    forasync_t base;
    loop_domain_t loop0;
    loop_domain_t loop1;
} forasync2D_t;

typedef struct _forasync_2D_task_t {
    forasync_task_t task;
    forasync2D_t def;
} forasync2D_task_t;

typedef struct {
    forasync_t base;
    loop_domain_t loop0;
    loop_domain_t loop1;
    loop_domain_t loop2;
} forasync3D_t;

typedef struct _forasync_3D_task_t {
    forasync_task_t task;
    forasync3D_t def;
} forasync3D_task_t;


#endif /* HCLIB_DEF_H_ */
