#include "idle.h"

#include "../cpu/cpu.h"

/*
 * ============================================================
 * XyrisOS Idle Thread
 * ============================================================
 *
 * Runs whenever there are no runnable threads.
 */

/*
 * ------------------------------------------------------------
 * Initialize Idle Thread
 * ------------------------------------------------------------
 */

void idle_initialize(void)
{
    /*
     * Nothing to initialize yet.
     *
     * Future:
     * - Create idle process
     * - Create idle thread
     * - Register with scheduler
     */
}

/*
 * ------------------------------------------------------------
 * Idle Thread
 * ------------------------------------------------------------
 */

void idle_thread(void)
{
    while (1)
    {
        cpu_halt();
    }
}