#ifndef XYRIS_SCHEDULER_H
#define XYRIS_SCHEDULER_H

#include <stdint.h>

#include "thread.h"

/*
 * ============================================================
 * XyrisOS Scheduler
 * ------------------------------------------------------------
 * Round-Robin Scheduler Interface
 * ============================================================
 */

/*
 * ------------------------------------------------------------
 * Scheduler Configuration
 * ------------------------------------------------------------
 */

#define SCHEDULER_DEFAULT_TIME_SLICE    10

/*
 * ------------------------------------------------------------
 * Scheduler Structure
 * ------------------------------------------------------------
 */

typedef struct scheduler
{
    /*
     * Currently executing thread
     */

    thread_t *current;

    /*
     * Ready queue
     */

    thread_t *ready_head;

    thread_t *ready_tail;

    /*
     * Sleeping queue
     */

    thread_t *sleep_head;

    /*
     * Statistics
     */

    uint64_t total_threads;

    uint64_t context_switches;

    uint64_t ticks;

    /*
     * Time slice
     */

    uint32_t time_slice;

} scheduler_t;

/*
 * ------------------------------------------------------------
 * Scheduler API
 * ------------------------------------------------------------
 */

void scheduler_initialize(void);

void scheduler_start(void);

void scheduler_tick(void);

void scheduler_schedule(void);

void scheduler_add_thread(
    thread_t *thread
);

void scheduler_remove_thread(
    thread_t *thread
);

thread_t *scheduler_current_thread(void);

void scheduler_yield(void);

#endif