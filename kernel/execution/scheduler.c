/*
 * scheduler.c
 * XyrisOS Kernel
 *
 * Phase 1 Round-Robin Scheduler
 */

#include "scheduler.h"

static thread_t* ready_head = NULL;
static thread_t* ready_tail = NULL;
static thread_t* current = NULL;

/* --------------------------------------------------
   Initialize Scheduler
-------------------------------------------------- */

void scheduler_init(void)
{
    ready_head = NULL;
    ready_tail = NULL;
    current = NULL;
}

/* --------------------------------------------------
   Add Thread
-------------------------------------------------- */

void scheduler_add(thread_t* thread)
{
    if (thread == NULL)
        return;

    thread->next_all = NULL;

    if (ready_head == NULL)
    {
        ready_head = thread;
        ready_tail = thread;
        return;
    }

    ready_tail->next_all = thread;
    ready_tail = thread;
}

/* --------------------------------------------------
   Remove Thread
-------------------------------------------------- */

void scheduler_remove(thread_t* thread)
{
    if (thread == NULL)
        return;

    if (ready_head == thread)
    {
        ready_head = thread->next_all;

        if (ready_tail == thread)
            ready_tail = NULL;

        return;
    }

    thread_t* current_thread = ready_head;

    while (current_thread != NULL &&
           current_thread->next_all != thread)
    {
        current_thread = current_thread->next_all;
    }

    if (current_thread == NULL)
        return;

    current_thread->next_all = thread->next_all;

    if (ready_tail == thread)
        ready_tail = current_thread;
}

/* --------------------------------------------------
   Select Next Thread
-------------------------------------------------- */

thread_t* scheduler_next(void)
{
    if (ready_head == NULL)
        return NULL;

    current = ready_head;

    ready_head = ready_head->next_all;

    current->next_all = NULL;

    if (ready_head == NULL)
    {
        ready_head = current;
        ready_tail = current;
    }
    else
    {
        ready_tail->next_all = current;
        ready_tail = current;
    }

    return current;
}

/* --------------------------------------------------
   Timer Tick
-------------------------------------------------- */

void scheduler_tick(void)
{
    /*
     * Phase 1:
     * Simply advance to the next thread.
     *
     * Timer interrupts will call this later.
     */

    scheduler_next();
}

/* --------------------------------------------------
   Current Thread
-------------------------------------------------- */

thread_t* scheduler_current(void)
{
    return current;
}