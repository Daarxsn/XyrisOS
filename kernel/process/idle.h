#ifndef XYRIS_IDLE_H
#define XYRIS_IDLE_H

/*
 * ============================================================
 * XyrisOS Idle Thread
 * ------------------------------------------------------------
 * The idle thread executes whenever there are no runnable
 * threads in the system.
 * ============================================================
 */

/*
 * Initialize idle thread subsystem.
 */
void idle_initialize(void);

/*
 * Idle thread entry point.
 */
void idle_thread(void);

#endif