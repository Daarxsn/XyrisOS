/*
 * execution.c
 * XyrisOS Kernel
 *
 * Execution Manager
 */

#include "execution.h"

static thread_t* thread_list = NULL;

void execution_init(void)
{
    thread_init();

    thread_list = NULL;
}

thread_t* execution_create(void)
{
    thread_t* thread = thread_create();

    if (thread == NULL)
        return NULL;

    if (thread_list == NULL)
    {
        thread_list = thread;
    }
    else
    {
        thread_t* current = thread_list;

        while (current->next_all != NULL)
        {
            current = current->next_all;
        }

        current->next_all = thread;
    }

    return thread;
}

void execution_destroy(thread_t* thread)
{
    if (thread == NULL)
        return;

    if (thread == thread_list)
    {
        thread_list = thread->next_all;
    }
    else
    {
        thread_t* current = thread_list;

        while (current != NULL &&
               current->next_all != thread)
        {
            current = current->next_all;
        }

        if (current != NULL)
            current->next_all = thread->next_all;
    }

    thread_destroy(thread);
}

void execution_yield(void)
{
    /*
     * Phase 1
     *
     * Scheduler will implement this.
     */
}

thread_t* execution_first(void)
{
    return thread_list;
}