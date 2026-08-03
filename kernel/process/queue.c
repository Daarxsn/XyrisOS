#include "queue.h"

#include <stddef.h>

/*
 * ============================================================
 * XyrisOS Thread Queue
 * ============================================================
 */

/*
 * ------------------------------------------------------------
 * Initialize Queue
 * ------------------------------------------------------------
 */

void queue_initialize(thread_queue_t *queue)
{
    if (queue == NULL)
    {
        return;
    }

    queue->head = NULL;
    queue->tail = NULL;
    queue->count = 0;
}

/*
 * ------------------------------------------------------------
 * Queue Empty?
 * ------------------------------------------------------------
 */

bool queue_is_empty(const thread_queue_t *queue)
{
    if (queue == NULL)
    {
        return true;
    }

    return (queue->count == 0);
}

/*
 * ------------------------------------------------------------
 * Push Thread
 * ------------------------------------------------------------
 */

void queue_push(
    thread_queue_t *queue,
    thread_t *thread)
{
    if (queue == NULL || thread == NULL)
    {
        return;
    }

    thread->next = NULL;
    thread->previous = NULL;

    if (queue->tail == NULL)
    {
        queue->head = thread;
        queue->tail = thread;
    }
    else
    {
        thread->previous = queue->tail;

        queue->tail->next = thread;

        queue->tail = thread;
    }

    queue->count++;
}

/*
 * ------------------------------------------------------------
 * Pop Thread
 * ------------------------------------------------------------
 */

thread_t *queue_pop(thread_queue_t *queue)
{
    if (queue == NULL)
    {
        return NULL;
    }

    if (queue->head == NULL)
    {
        return NULL;
    }

    thread_t *thread = queue->head;

    queue->head = thread->next;

    if (queue->head != NULL)
    {
        queue->head->previous = NULL;
    }
    else
    {
        queue->tail = NULL;
    }

    thread->next = NULL;
    thread->previous = NULL;

    queue->count--;

    return thread;
}

/*
 * ------------------------------------------------------------
 * Remove Thread
 * ------------------------------------------------------------
 */

void queue_remove(
    thread_queue_t *queue,
    thread_t *thread)
{
    if (queue == NULL || thread == NULL)
    {
        return;
    }

    if (thread->previous != NULL)
    {
        thread->previous->next = thread->next;
    }
    else
    {
        queue->head = thread->next;
    }

    if (thread->next != NULL)
    {
        thread->next->previous = thread->previous;
    }
    else
    {
        queue->tail = thread->previous;
    }

    thread->next = NULL;
    thread->previous = NULL;

    if (queue->count > 0)
    {
        queue->count--;
    }
}