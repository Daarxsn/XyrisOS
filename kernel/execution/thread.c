/*
 * thread.c
 * XyrisOS Kernel
 */

#include "thread.h"
#include "../memory/heap.h"

#include <stddef.h>

static uint64_t next_tid = 1;

void thread_init(void)
{
    next_tid = 1;
}

thread_t* thread_create(void (*entry)(void))
{
    thread_t* thread = (thread_t*)kmalloc(sizeof(thread_t));

    if (thread == NULL)
        return NULL;

    thread->id = next_tid++;

    thread->stack = NULL;

    thread->rsp = 0;
    thread->rip = 0;

    thread->state = THREAD_READY;

    thread->next_all = NULL;
    thread->next_ready = NULL;

    return thread;
}

void thread_destroy(thread_t* thread)
{
    if (thread == NULL)
        return;

    thread->state = THREAD_TERMINATED;

    kfree(thread);
}