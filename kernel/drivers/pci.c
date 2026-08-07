#include "drivers/pci.h"

#include "cpu/io.h"

static bool pci_initialized = false;

/* ------------------------------------------------------------
 * PCI Configuration Read
 * ------------------------------------------------------------ */

uint32_t xk_pci_read32(
    uint8_t bus,
    uint8_t device,
    uint8_t function,
    uint8_t offset)
{
    uint32_t address =
        (1U << 31) |
        ((uint32_t)bus << 16) |
        ((uint32_t)device << 11) |
        ((uint32_t)function << 8) |
        (offset & 0xFC);

    outl(PCI_CONFIG_ADDRESS, address);

    return inl(PCI_CONFIG_DATA);
}

/* ------------------------------------------------------------
 * Vendor ID
 * ------------------------------------------------------------ */

uint16_t xk_pci_vendor_id(
    uint8_t bus,
    uint8_t device,
    uint8_t function)
{
    return (uint16_t)
        (xk_pci_read32(bus, device, function, 0) & 0xFFFF);
}

/* ------------------------------------------------------------
 * Scan
 * ------------------------------------------------------------ */

void xk_pci_scan(void)
{
    for (uint16_t bus = 0; bus < 256; bus++)
    {
        for (uint8_t device = 0; device < 32; device++)
        {
            uint16_t vendor =
                xk_pci_vendor_id(bus, device, 0);

            if (vendor == 0xFFFF)
            {
                continue;
            }

            /*
             * Phase 2:
             * Store device information.
             */
        }
    }
}

/* ------------------------------------------------------------
 * Initialize
 * ------------------------------------------------------------ */

bool xk_pci_initialize(void)
{
    pci_initialized = true;

    xk_pci_scan();

    return true;
}

/* ------------------------------------------------------------
 * Shutdown
 * ------------------------------------------------------------ */

void xk_pci_shutdown(void)
{
    pci_initialized = false;
}

/* ------------------------------------------------------------
 * Driver Object
 * ------------------------------------------------------------ */

XKDriver xk_pci_driver =
{
    .name = "PCI Enumerator",
    .type = XK_DRIVER_CUSTOM,
    .state = XK_DRIVER_UNINITIALIZED,
    .initialize = xk_pci_initialize,
    .shutdown = xk_pci_shutdown
};