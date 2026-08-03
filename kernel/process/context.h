#ifndef XYRIS_CONTEXT_H
#define XYRIS_CONTEXT_H

#include "thread.h"

/*
 * ============================================================
 * XyrisOS Context Switching
 * ------------------------------------------------------------
 * Saves and restores CPU execution contexts.
 * ============================================================
 */

/*
 * ------------------------------------------------------------
 * Context Switch API
 * ------------------------------------------------------------
 */

/*
 * Save the current thread context.
 */
void context_save(
    thread_t *thread
);

/*
 * Restore a previously saved thread context.
 */
void context_restore(
    thread_t *thread
);

/*
 * Switch execution from one thread to another.
 */
void context_switch(
    thread_t *current,
    thread_t *next
);

#endif