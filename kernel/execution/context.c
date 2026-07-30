#include "thread.h"
#include "scheduler.h"

void thread_bootstrap(void)
{
    thread_t* current = scheduler_current();

    current->entry(current->argument);

    current->state = THREAD_TERMINATED;

    for (;;)
        __asm__ volatile("hlt");
}