#include "context.h"

#include <stddef.h>

/*
 * ============================================================
 * XyrisOS Context Manager
 * ============================================================
 *
 * This is the architecture-independent context framework.
 *
 * NOTE:
 * Real register save/restore will be implemented later
 * in assembly during the hardware context switching phase.
 */

/*
 * ------------------------------------------------------------
 * Initialize Context Manager
 * ------------------------------------------------------------
 */

void context_initialize(void)
{
    /*
     * Nothing to initialize yet.
     *
     * Future:
     * - Per-CPU context storage
     * - Architecture initialization
     */
}

/*
 * ------------------------------------------------------------
 * Save Context
 * ------------------------------------------------------------
 */

void context_save(context_t *context)
{
    if (context == NULL)
    {
        return;
    }

    /*
     * Stub implementation.
     *
     * Real CPU register saving
     * will be implemented in
     * the assembly context switch.
     */
}

/*
 * ------------------------------------------------------------
 * Restore Context
 * ------------------------------------------------------------
 */

void context_restore(context_t *context)
{
    if (context == NULL)
    {
        return;
    }

    /*
     * Stub implementation.
     *
     * Real register restore
     * will be implemented later.
     */
}

/*
 * ------------------------------------------------------------
 * Context Switch
 * ------------------------------------------------------------
 */

void context_switch(
    context_t *current,
    context_t *next)
{
    /*
     * Avoid unused parameter warnings.
     */

    (void)current;
    (void)next;

    /*
     * Phase 5:
     * Scheduler only.
     *
     * Phase 6:
     * Software context switching.
     *
     * Phase 7:
     * Hardware assembly context switching.
     */
}