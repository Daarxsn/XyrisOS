/*
 * scheduler.c
 * XyrisOS Kernel
 *
 * Phase 1 Round-Robin Scheduler
 */

#include "scheduler.h"
#include <stddef.h>
#include "switch.h"

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

    thread->next_ready = NULL;

    if (ready_head == NULL)
    {
        ready_head = thread;
        ready_tail = thread;
        return;
    }

    ready_tail->next_ready = thread;
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
        ready_head = thread->next_ready;

        if (ready_tail == thread)
            ready_tail = NULL;

        return;
    }

    thread_t* current_thread = ready_head;

    while (current_thread != NULL &&
           current_thread->next_ready != thread)
    {
        current_thread = current_thread->next_ready;
    }

    if (current_thread == NULL)
        return;

    current_thread->next_ready = thread->next_ready;

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

    ready_head = ready_head->next_ready;

    current->next_ready = NULL;

    if (ready_head == NULL)
    {
        ready_head = current;
        ready_tail = current;
    }
    else
    {
        ready_tail->next_ready = current;
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

    thread_t* previous = scheduler_current();
    thread_t* next = scheduler_next();

    if (previous == NULL || next == NULL)
        return;

    if (previous == next)
        return;

    switch_context(
        &previous->context,
        &next->context);
}

/* --------------------------------------------------
   Current Thread
-------------------------------------------------- */

thread_t* scheduler_current(void)
{
    return current;
}