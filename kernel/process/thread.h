#ifndef XYRIS_THREAD_H
#define XYRIS_THREAD_H

#include <stdint.h>
#include <stdbool.h>

#include "process.h"

/*
 * ============================================================
 * XyrisOS Thread Manager
 * ------------------------------------------------------------
 * Defines the Thread Control Block (TCB) and public API.
 * ============================================================
 */

/*
 * ------------------------------------------------------------
 * Configuration
 * ------------------------------------------------------------
 */

#define THREAD_MAX_COUNT     1024
#define THREAD_STACK_SIZE    (16 * 1024)

/*
 * ------------------------------------------------------------
 * Thread Identifier
 * ------------------------------------------------------------
 */

typedef uint32_t thread_id_t;

/*
 * ------------------------------------------------------------
 * Thread State
 * ------------------------------------------------------------
 */

typedef enum
{
    THREAD_CREATED = 0,

    THREAD_READY,

    THREAD_RUNNING,

    THREAD_BLOCKED,

    THREAD_SLEEPING,

    THREAD_TERMINATED

} thread_state_t;

/*
 * ------------------------------------------------------------
 * CPU Context
 *
 * This will later be saved/restored by the scheduler.
 * ------------------------------------------------------------
 */

typedef struct cpu_context
{
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;

    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;

    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;

    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;

    uint64_t rip;
    uint64_t rsp;

    uint64_t rflags;

} cpu_context_t;

/*
 * ------------------------------------------------------------
 * Thread Control Block (TCB)
 * ------------------------------------------------------------
 */

typedef struct thread
{
    /*
     * Identification
     */

    thread_id_t tid;

    /*
     * Owner process
     */

    process_t *owner;

    /*
     * Current state
     */

    thread_state_t state;

    /*
     * Saved CPU registers
     */

    cpu_context_t context;

    /*
     * Stack
     */

    void *stack_base;

    void *stack_top;

    /*
     * Scheduling
     */

    uint32_t priority;

    uint64_t cpu_time;

    uint64_t wakeup_tick;

    /*
     * Queue links
     */

    struct thread *next;

    struct thread *previous;

} thread_t;

/*
 * ------------------------------------------------------------
 * Thread Manager API
 * ------------------------------------------------------------
 */

void thread_manager_initialize(void);

thread_t *thread_create(
    process_t *owner,
    void (*entry)(void),
    uint32_t priority
);

void thread_destroy(
    thread_t *thread
);

thread_t *thread_current(void);

void thread_set_state(
    thread_t *thread,
    thread_state_t state
);

thread_state_t thread_get_state(
    const thread_t *thread
);

#endif