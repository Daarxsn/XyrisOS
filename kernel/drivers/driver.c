#include "drivers/driver.h"

#include <stddef.h>
#include <string.h>

/* ============================================================
 * Driver Table
 * ============================================================
 */

static XKDriver *driver_table[XK_MAX_DRIVERS];

/* ------------------------------------------------------------
 * Initialize Driver Manager
 * ------------------------------------------------------------ */

void xk_driver_manager_init(void)
{
    for (uint32_t i = 0; i < XK_MAX_DRIVERS; i++)
    {
        driver_table[i] = NULL;
    }
}

/* ------------------------------------------------------------
 * Register Driver
 * ------------------------------------------------------------ */

bool xk_driver_register(XKDriver *driver)
{
    if (driver == NULL)
    {
        return false;
    }

    for (uint32_t i = 0; i < XK_MAX_DRIVERS; i++)
    {
        if (driver_table[i] == NULL)
        {
            driver->state = XK_DRIVER_REGISTERED;
            driver_table[i] = driver;
            return true;
        }
    }

    return false;
}

/* ------------------------------------------------------------
 * Find Driver
 * ------------------------------------------------------------ */

XKDriver *xk_driver_find(const char *name)
{
    if (name == NULL)
    {
        return NULL;
    }

    for (uint32_t i = 0; i < XK_MAX_DRIVERS; i++)
    {
        if (driver_table[i] != NULL &&
            strcmp(driver_table[i]->name, name) == 0)
        {
            return driver_table[i];
        }
    }

    return NULL;
}

/* ------------------------------------------------------------
 * Initialize All Drivers
 * ------------------------------------------------------------ */

void xk_driver_initialize_all(void)
{
    for (uint32_t i = 0; i < XK_MAX_DRIVERS; i++)
    {
        if (driver_table[i] == NULL)
            continue;

        if (driver_table[i]->initialize != NULL)
        {
            if (driver_table[i]->initialize())
            {
                driver_table[i]->state =
                    XK_DRIVER_INITIALIZED;
            }
            else
            {
                driver_table[i]->state =
                    XK_DRIVER_FAILED;
            }
        }
    }
}

/* ------------------------------------------------------------
 * Shutdown All Drivers
 * ------------------------------------------------------------ */

void xk_driver_shutdown_all(void)
{
    for (uint32_t i = 0; i < XK_MAX_DRIVERS; i++)
    {
        if (driver_table[i] == NULL)
            continue;

        if (driver_table[i]->shutdown != NULL)
        {
            driver_table[i]->shutdown();
        }
    }
}