#ifndef XYRIS_IDLE_H
#define XYRIS_IDLE_H

/*
 * ============================================================
 * XyrisOS Idle Process
 * ------------------------------------------------------------
 * The idle thread executes whenever there are no runnable
 * threads in the scheduler.
 *
 * The idle thread is never destroyed and always exists.
 * ============================================================
 */

#include "thread.h"

/*
 * ------------------------------------------------------------
 * Idle Thread API
 * ------------------------------------------------------------
 */

/*
 * Initialize the idle thread.
 */
void idle_initialize(void);

/*
 * Return the idle thread.
 */
thread_t *idle_thread(void);

/*
 * Idle thread entry function.
 */
void idle_entry(void);

#endif