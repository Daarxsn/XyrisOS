#include "scheduler.h"

#include <stddef.h>

/*
 * ============================================================
 * XyrisOS Scheduler
 * ============================================================
 */

/*
 * ------------------------------------------------------------
 * Scheduler Instance
 * ------------------------------------------------------------
 */

static scheduler_t scheduler;

/*
 * ------------------------------------------------------------
 * Initialize Scheduler
 * ------------------------------------------------------------
 */

void scheduler_initialize(void)
{
    scheduler.current = NULL;

    queue_initialize(&scheduler.ready_queue);
    queue_initialize(&scheduler.blocked_queue);
    queue_initialize(&scheduler.sleeping_queue);

    scheduler.ticks = 0;
    scheduler.context_switches = 0;
    scheduler.total_threads = 0;

    scheduler.time_slice =
        SCHEDULER_DEFAULT_TIME_SLICE;
}

/*
 * ------------------------------------------------------------
 * Start Scheduler
 * ------------------------------------------------------------
 */

void scheduler_start(void)
{
    /*
     * Scheduler starts automatically
     * once runnable threads exist.
     */
}

/*
 * ------------------------------------------------------------
 * Scheduler Tick
 * ------------------------------------------------------------
 */

void scheduler_tick(void)
{
    scheduler.ticks++;
}

/*
 * ------------------------------------------------------------
 * Add Thread
 * ------------------------------------------------------------
 */

void scheduler_add_thread(thread_t *thread)
{
    if (thread == NULL)
    {
        return;
    }

    thread->state = THREAD_READY;

    queue_push(
        &scheduler.ready_queue,
        thread
    );

    scheduler.total_threads++;
}

/*
 * ------------------------------------------------------------
 * Remove Thread
 * ------------------------------------------------------------
 */

void scheduler_remove_thread(thread_t *thread)
{
    if (thread == NULL)
    {
        return;
    }

    queue_remove(
        &scheduler.ready_queue,
        thread
    );

    if (scheduler.total_threads > 0)
    {
        scheduler.total_threads--;
    }
}

/*
 * ------------------------------------------------------------
 * Current Thread
 * ------------------------------------------------------------
 */

thread_t *scheduler_current_thread(void)
{
    return scheduler.current;
}

/*
 * ------------------------------------------------------------
 * Yield CPU
 * ------------------------------------------------------------
 */

void scheduler_yield(void)
{
    scheduler_schedule();
}

/*
 * ------------------------------------------------------------
 * Round Robin Scheduler
 * ------------------------------------------------------------
 */

void scheduler_schedule(void)
{
    /*
     * Put current thread
     * back into ready queue.
     */

    if (scheduler.current != NULL)
    {
        scheduler.current->state =
            THREAD_READY;

        queue_push(
            &scheduler.ready_queue,
            scheduler.current
        );
    }

    /*
     * Select next thread.
     */

    thread_t *next =
        queue_pop(
            &scheduler.ready_queue
        );

    if (next == NULL)
    {
        return;
    }

    /*
     * Context switching will
     * be added in Phase 5.10.
     */

    scheduler.current = next;

    thread_set_current(next);

    next->state =
        THREAD_RUNNING;

    scheduler.context_switches++;
}