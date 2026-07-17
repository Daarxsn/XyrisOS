#ifndef XYRIS_THREAD_H
#define XYRIS_THREAD_H

#include <stdint.h>

typedef enum
{
    THREAD_READY,
    THREAD_RUNNING,
    THREAD_BLOCKED,
    THREAD_TERMINATED

} thread_state_t;

typedef struct thread
{
    uint64_t id;

    void* stack;

    uint64_t rsp;
    uint64_t rip;

    thread_state_t state;

    struct thread* next_all;
    struct thread* next_ready;

} thread_t;

void thread_init(void);

thread_t* thread_create(void);

void thread_destroy(thread_t* thread);

#endif