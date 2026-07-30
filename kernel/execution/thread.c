/*
 * thread.c
 * XyrisOS Kernel
 */

#include "thread.h"
#include "../memory/heap.h"
#include "../memory/pmm.h"
#include "../memory/hhdm.h"

#include <stddef.h>

void thread_bootstrap(void);

static uint64_t next_tid = 1;

void thread_init(void)
{
    next_tid = 1;
}


thread_create(
    void (*entry)(void*),
    void* argument)
{
    thread_t* thread = (thread_t*)kmalloc(sizeof(thread_t));

    if (thread == NULL)
        return NULL;

    thread->id = next_tid++;

    thread->entry = entry;
    thread->argument = argument;

    thread->entry = NULL;
    thread->argument = NULL;

    phys_addr_t stack_phys =
        pmm_alloc_pages(THREAD_STACK_PAGES);

    if (stack_phys == 0)
    {
        kfree(thread);
        return NULL;
    }

    thread->stack_base =
        (uintptr_t)phys_to_virt(stack_phys);

    thread->stack_size = THREAD_STACK_SIZE;

thread->stack_top =
    thread->stack_base + THREAD_STACK_SIZE;

    /* Build initial stack frame */
        uintptr_t* stack =
            (uintptr_t*)thread->stack_top;

        --stack;
        *stack = 0;

    thread->context.rsp = (uintptr_t)stack;
    thread->context.rip = (uintptr_t)thread_bootstrap;

    memset(&thread->context, 0, sizeof(context_t));

    thread->context.rsp = thread->stack_top;

    thread->state = THREAD_READY;

    thread->next_all = NULL;
    thread->next_ready = NULL;

    return thread;
}

void thread_destroy(thread_t* thread)
{

    /*
    * Stack pages are intentionally leaked in Phase 1.
    * PMM deallocation will be added later.
    */
    if (thread == NULL)
        return;

    thread->state = THREAD_TERMINATED;

    kfree(thread);
}