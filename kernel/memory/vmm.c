#include "vmm.h"

#include "pmm.h"
#include "hhdm.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* --------------------------------------------------
   x86-64 Paging Constants
-------------------------------------------------- */

#define ENTRIES_PER_TABLE 512

#define PAGE_SHIFT      12
#define PAGE_SIZE  (1ULL << PAGE_SHIFT)
#define PAGE_MASK       0xFFFFFFFFFFFFF000ULL

#define PML4_INDEX(x) (((x) >> 39) & 0x1FF)
#define PDPT_INDEX(x) (((x) >> 30) & 0x1FF)
#define PD_INDEX(x)   (((x) >> 21) & 0x1FF)
#define PT_INDEX(x)   (((x) >> 12) & 0x1FF)

#define PAGE_PRESENT   (1ULL << 0)
#define PAGE_WRITABLE  (1ULL << 1)
#define PAGE_USER      (1ULL << 2)

/* --------------------------------------------------
   Internal Address Space
-------------------------------------------------- */

struct address_space
{
    uint64_t* pml4;
};

/* --------------------------------------------------
   Kernel Address Space
-------------------------------------------------- */

static struct address_space kernel_space;

/* --------------------------------------------------
   Helpers
-------------------------------------------------- */

static inline uint64_t* current_pml4(void)
{
    uintptr_t cr3 = read_cr3();

    /*
     * CR3 stores the physical address of the PML4.
     * Convert it into a kernel virtual address
     * using the Higher Half Direct Map.
     */
    return (uint64_t*)phys_to_virt(cr3);
}

static uint64_t* page_table_from_entry(uint64_t entry)
{
    return (uint64_t*)phys_to_virt(entry & PAGE_MASK);
}

static uint64_t* allocate_table(void)
{
    phys_addr_t phys = pmm_alloc_page();

    if (phys == 0)
        return NULL;

    uint64_t* table = phys_to_virt(phys);

    memset(table, 0, PAGE_SIZE);

    return table;
}

static uint64_t* walk_page_tables(
    address_space_t* space,
    uintptr_t virtual_addr,
    bool create)
{
    uint64_t* pml4 = space->pml4;

    /* ---------- PML4 ---------- */

    uint64_t* pdpt;

    if (!(pml4[PML4_INDEX(virtual_addr)] & PAGE_PRESENT))
    {
        if (!create)
            return NULL;

        pdpt = allocate_table();

        if (pdpt == NULL)
            return NULL;

        pml4[PML4_INDEX(virtual_addr)] =
            virt_to_phys(pdpt)
            | PAGE_PRESENT
            | PAGE_WRITABLE;
    }
    else
    {
        pdpt = page_table_from_entry(
            pml4[PML4_INDEX(virtual_addr)]);
    }

    /* ---------- PDPT ---------- */

    uint64_t* pd;

    if (!(pdpt[PDPT_INDEX(virtual_addr)] & PAGE_PRESENT))
    {
        if (!create)
            return NULL;

        pd = allocate_table();

        if (pd == NULL)
            return NULL;

        pdpt[PDPT_INDEX(virtual_addr)] =
            virt_to_phys(pd)
            | PAGE_PRESENT
            | PAGE_WRITABLE;
    }
    else
    {
        pd = page_table_from_entry(
            pdpt[PDPT_INDEX(virtual_addr)]);
    }

    /* ---------- PD ---------- */

    uint64_t* pt;

    if (!(pd[PD_INDEX(virtual_addr)] & PAGE_PRESENT))
    {
        if (!create)
            return NULL;

        pt = allocate_table();

        if (pt == NULL)
            return NULL;

        pd[PD_INDEX(virtual_addr)] =
            virt_to_phys(pt)
            | PAGE_PRESENT
            | PAGE_WRITABLE;
    }
    else
    {
        pt = page_table_from_entry(
            pd[PD_INDEX(virtual_addr)]);
    }

    return pt;
}

static inline uintptr_t read_cr3(void)
{
    uintptr_t value;

    __asm__ volatile(
        "mov %%cr3, %0"
        : "=r"(value));

    return value;
}

static inline void write_cr3(uintptr_t value)
{
    __asm__ volatile(
        "mov %0, %%cr3"
        :
        : "r"(value)
        : "memory");
}

address_space_t* vmm_kernel_space(void)
{
    return &kernel_space;
}

void vmm_init(void)
{
    kernel_space.pml4 = current_pml4();
}


void vmm_flush(uintptr_t virtual_addr)
{
    __asm__ volatile(
        "invlpg (%0)"
        :
        : "r"(virtual_addr)
        : "memory");
}

void vmm_switch_space(address_space_t* space)
{
    if (space == NULL)
        return;

    write_cr3(
        virt_to_phys(space->pml4));
}

bool vmm_map_page(
    address_space_t* space,
    uintptr_t virtual_addr,
    phys_addr_t physical_addr,
    uint64_t flags)
{
    if (space == NULL)
        return false;

    uint64_t* pt = walk_page_tables(space, virtual_addr, true);

    if (pt == NULL)
        return false;

    size_t index = PT_INDEX(virtual_addr);

    if (pt[index] & VMM_PRESENT)
        return false;      // Already mapped

    pt[index] = (physical_addr & PAGE_MASK)
              | flags
              | VMM_PRESENT;

    vmm_flush(virtual_addr);

    return true;
}

bool vmm_unmap_page(
    address_space_t* space,
    uintptr_t virtual_addr)
{
    if (space == NULL)
        return false;

    uint64_t* pt = walk_page_tables(space, virtual_addr, false);

    if (pt == NULL)
        return false;

    size_t index = PT_INDEX(virtual_addr);

    if (!(pt[index] & VMM_PRESENT))
        return false;

    pt[index] = 0;

    vmm_flush(virtual_addr);

    return true;
}

phys_addr_t vmm_translate(
    address_space_t* space,
    uintptr_t virtual_addr)
{
    if (space == NULL)
        return 0;

    uint64_t* pt = walk_page_tables(space, virtual_addr, false);

    if (pt == NULL)
        return 0;

    uint64_t entry = pt[PT_INDEX(virtual_addr)];

    if (!(entry & VMM_PRESENT))
        return 0;

    return (entry & PAGE_MASK) | (virtual_addr & 0xFFF);
}

address_space_t* vmm_create_space(void)
{
    struct address_space* space =
        kmalloc(sizeof(struct address_space));

    if (space == NULL)
        return NULL;

    uint64_t* pml4 = allocate_table();

    if (pml4 == NULL)
    {
        /* kfree() is currently a stub, so this leaks one small allocation.
           That's acceptable in Phase 1 and can be cleaned up later. */
        return NULL;
    }

    /* Copy kernel mappings (upper half). */
    for (size_t i = 256; i < ENTRIES_PER_TABLE; i++)
    {
        pml4[i] = kernel_space.pml4[i];
    }

    space->pml4 = pml4;

    return space;
}

void vmm_destroy_space(address_space_t* space)
{
    if (space == NULL)
        return;

    /*
     * Phase 1:
     *  - Don't free page tables.
     *  - Don't free mapped pages.
     *  - Don't free the structure.
     *
     * These will be implemented once the heap and PMM support
     * proper deallocation.
     */
}