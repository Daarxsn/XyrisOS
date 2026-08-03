#include "thread.h"

#include <stddef.h>
#include "../lib/string.h"

/*
 * ============================================================
 * XyrisOS Thread Manager
 * ============================================================
 */

/*
 * ------------------------------------------------------------
 * Thread Table
 * ------------------------------------------------------------
 */

static thread_t thread_table[THREAD_MAX_COUNT];

static thread_t *current_thread = NULL;

static thread_id_t next_tid = 1;

/*
 * ------------------------------------------------------------
 * Initialize Thread Manager
 * ------------------------------------------------------------
 */

void thread_initialize(void)
{
    memset(thread_table, 0, sizeof(thread_table));

    current_thread = NULL;

    next_tid = 1;
}

/*
 * ------------------------------------------------------------
 * Create Thread
 * ------------------------------------------------------------
 */

thread_t *thread_create(
    process_t *owner,
    void (*entry)(void),
    thread_priority_t priority)
{
    if (owner == NULL || entry == NULL)
    {
        return NULL;
    }

    for (uint32_t i = 0; i < THREAD_MAX_COUNT; i++)
    {
        if (thread_table[i].tid == 0)
        {
            thread_t *thread = &thread_table[i];

            memset(thread, 0, sizeof(thread_t));

            thread->tid = next_tid++;

            thread->owner = owner;

            thread->entry = entry;

            thread->state = THREAD_CREATED;

            thread->priority = priority;

            /*
             * Context and stack are initialized
             * in later phases.
             */

            thread->context = NULL;

            thread->stack = NULL;

            thread->stack_size = THREAD_STACK_SIZE;

            thread->cpu_time = 0;

            thread->time_slice = 0;

            thread->next = NULL;

            thread->previous = NULL;

            /*
             * First thread becomes
             * the process main thread.
             */

            if (owner->main_thread == NULL)
            {
                owner->main_thread = thread;
            }

            return thread;
        }
    }

    return NULL;
}

/*
 * ------------------------------------------------------------
 * Destroy Thread
 * ------------------------------------------------------------
 */

void thread_destroy(thread_t *thread)
{
    if (thread == NULL)
    {
        return;
    }

    memset(thread, 0, sizeof(thread_t));
}

/*
 * ------------------------------------------------------------
 * Current Thread
 * ------------------------------------------------------------
 */

thread_t *thread_current(void)
{
    return current_thread;
}

/*
 * ------------------------------------------------------------
 * Set Current Thread
 * ------------------------------------------------------------
 */

void thread_set_current(thread_t *thread)
{
    current_thread = thread;
}