#include "vmm.h"

#include "pmm.h"
#include "heap.h"
#include "hhdm.h"
#include "../debug/print.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

/* --------------------------------------------------
   x86-64 Paging Constants
-------------------------------------------------- */

#define ENTRIES_PER_TABLE 512

#define PAGE_SHIFT      12
#define PAGE_MASK 0xFFFFFFFFFFFFF000ULL

#define PML4_INDEX(x) (((x) >> 39) & 0x1FF)
#define PDPT_INDEX(x) (((x) >> 30) & 0x1FF)
#define PD_INDEX(x)   (((x) >> 21) & 0x1FF)
#define PT_INDEX(x)   (((x) >> 12) & 0x1FF)

#define VMM_PRESENT   (1ULL << 0)
#define VMM_WRITABLE  (1ULL << 1)
#define VMM_USER      (1ULL << 2)

#define VMM_PWT       (1ULL << 3)
#define VMM_PCD       (1ULL << 4)
#define VMM_GLOBAL    (1ULL << 8)
#define VMM_HUGE      (1ULL << 7)
#define VMM_NX        (1ULL << 63)

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

static inline uintptr_t read_cr3(void)
{
    uintptr_t value;

    __asm__ volatile(
        "mov %%cr3, %0"
        : "=r"(value)
    );

    return value;
}


static inline void write_cr3(uintptr_t value)
{
    __asm__ volatile(
        "mov %0, %%cr3"
        :
        : "r"(value)
        : "memory"
    );
}


static inline uint64_t* current_pml4(void)
{
    uintptr_t cr3 = read_cr3();

    /* Remove CR3 flags */
    cr3 &= PAGE_MASK;

    return (uint64_t*)phys_to_virt(cr3);
}


static inline phys_addr_t entry_address(uint64_t entry)
{
    return (phys_addr_t)(entry & PAGE_MASK);
}


static uint64_t* page_table_from_entry(uint64_t entry)
{
    return (uint64_t*)phys_to_virt(entry_address(entry));
}


static inline bool entry_present(uint64_t entry)
{
    return (entry & VMM_PRESENT) != 0;
}


static inline bool entry_huge(uint64_t entry)
{
    return (entry & VMM_HUGE) != 0;
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


/* --------------------------------------------------
   Page Table Walker
-------------------------------------------------- */

static uint64_t* walk_page_tables(
    address_space_t* space,
    uintptr_t virtual_addr,
    bool create)
{
    uint64_t* pml4 = space->pml4;


    /* ---------- PML4 ---------- */

    uint64_t* pdpt;


    if (!(pml4[PML4_INDEX(virtual_addr)] & VMM_PRESENT))
    {
        if (!create)
            return NULL;


        pdpt = allocate_table();

        if (pdpt == NULL)
            return NULL;


        pml4[PML4_INDEX(virtual_addr)] =
            virt_to_phys(pdpt)
            | VMM_PRESENT
            | VMM_WRITABLE;
    }
    else
    {
        pdpt = page_table_from_entry(
            pml4[PML4_INDEX(virtual_addr)]);
    }



    /* ---------- PDPT ---------- */

    uint64_t* pd;


    if (!(pdpt[PDPT_INDEX(virtual_addr)] & VMM_PRESENT))
    {
        if (!create)
            return NULL;


        pd = allocate_table();

        if (pd == NULL)
            return NULL;


        pdpt[PDPT_INDEX(virtual_addr)] =
            virt_to_phys(pd)
            | VMM_PRESENT
            | VMM_WRITABLE;
    }
    else
    {
        if (entry_huge(pdpt[PDPT_INDEX(virtual_addr)]))
        {
            debug_print("VMM: PDPT huge page detected\n");
            return NULL;
        }


        pd = page_table_from_entry(
            pdpt[PDPT_INDEX(virtual_addr)]);
    }



    /* ---------- PD ---------- */

    uint64_t* pt;


    if (!(pd[PD_INDEX(virtual_addr)] & VMM_PRESENT))
    {
        if (!create)
            return NULL;


        pt = allocate_table();

        if (pt == NULL)
            return NULL;


        pd[PD_INDEX(virtual_addr)] =
            virt_to_phys(pt)
            | VMM_PRESENT
            | VMM_WRITABLE;
    }
    else
    {
        if (pd[PD_INDEX(virtual_addr)] & VMM_HUGE)
        {
            debug_print("VMM: PD huge page detected\n");
            return NULL;
        }


        pt = page_table_from_entry(
            pd[PD_INDEX(virtual_addr)]);
    }


    return pt;
}
address_space_t* vmm_kernel_space(void)
{
    return &kernel_space;
}


void vmm_init(void)
{
    kernel_space.pml4 = current_pml4();

    if (kernel_space.pml4 == NULL)
    {
        debug_print("VMM: failed to get current PML4\n");
        return;
    }

    debug_print("VMM: initialized\n");
}



void vmm_flush(uintptr_t virtual_addr)
{
    __asm__ volatile(
        "invlpg (%0)"
        :
        : "r"(virtual_addr)
        : "memory"
    );
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
    {
        debug_print("VMM: NULL address space\n");
        return false;
    }


    if ((virtual_addr & (PAGE_SIZE - 1)) != 0)
    {
        debug_print("VMM: virtual address not aligned\n");
        return false;
    }


    if ((physical_addr & (PAGE_SIZE - 1)) != 0)
    {
        debug_print("VMM: physical address not aligned\n");
        return false;
    }



    uint64_t* pt =
        walk_page_tables(space, virtual_addr, true);



    if (pt == NULL)
    {
        debug_print("VMM: walk_page_tables failed\n");
        return false;
    }



    size_t index = PT_INDEX(virtual_addr);



    if (pt[index] & VMM_PRESENT)
    {
        debug_print("VMM: page already mapped\n");
        return false;
    }



    flags &= (
        VMM_WRITABLE |
        VMM_USER |
        VMM_PWT |
        VMM_PCD |
        VMM_GLOBAL |
        VMM_NX
    );



    pt[index] =
        (physical_addr & PAGE_MASK)
        | flags
        | VMM_PRESENT;



    vmm_flush(virtual_addr);



    debug_print("VMM: page mapped successfully\n");

    return true;
}

bool vmm_unmap_page(
    address_space_t* space,
    uintptr_t virtual_addr)
{
    if (space == NULL)
        return false;


    if ((virtual_addr & (PAGE_SIZE - 1)) != 0)
        return false;



    uint64_t* pt =
        walk_page_tables(space, virtual_addr, false);



    if (pt == NULL)
        return false;



    size_t index = PT_INDEX(virtual_addr);



    if (!(pt[index] & VMM_PRESENT))
        return false;



    pt[index] = 0;


    vmm_flush(virtual_addr);


    return true;
}

bool vmm_protect_page(
    address_space_t* space,
    uintptr_t virtual_addr,
    uint64_t flags)

    
{
    if (space == NULL)
        return false;

    if ((virtual_addr & (PAGE_SIZE - 1)) != 0)
        return false;

    uint64_t* pt =
        walk_page_tables(space, virtual_addr, false);

    if (pt == NULL)
        return false;

    size_t index = PT_INDEX(virtual_addr);

    if (!(pt[index] & VMM_PRESENT))
        return false;

    flags &= (
        VMM_WRITABLE |
        VMM_USER |
        VMM_PWT |
        VMM_PCD |
        VMM_GLOBAL |
        VMM_NX
    );

    uint64_t entry = pt[index];

    uint64_t preserve =
    entry & ~(VMM_WRITABLE |
              VMM_USER |
              VMM_PWT |
              VMM_PCD |
              VMM_GLOBAL |
              VMM_NX);

pt[index] = preserve | flags;

    vmm_flush(virtual_addr);

    return true;
}


uint64_t vmm_get_page_flags(
    address_space_t* space,
    uintptr_t virtual_addr)
{
    if (space == NULL)
        return 0;

    if ((virtual_addr & (PAGE_SIZE - 1)) != 0)
        return 0;

    uint64_t* pt =
        walk_page_tables(space, virtual_addr, false);

    if (pt == NULL)
        return 0;

    uint64_t entry =
        pt[PT_INDEX(virtual_addr)];

    if (!(entry & VMM_PRESENT))
        return 0;

    return entry &
        (VMM_PRESENT |
         VMM_WRITABLE |
         VMM_USER |
         VMM_PWT |
         VMM_PCD |
         VMM_ACCESSED |
         VMM_DIRTY |
         VMM_HUGE |
         VMM_GLOBAL |
         VMM_NX);
}


phys_addr_t vmm_translate(
    address_space_t* space,
    uintptr_t virtual_addr)
{
    if (space == NULL)
        return 0;


    if ((virtual_addr & (PAGE_SIZE - 1)) != 0)
        return 0;



    uint64_t* pt =
        walk_page_tables(space, virtual_addr, false);



    if (pt == NULL)
        return 0;



    uint64_t entry =
        pt[PT_INDEX(virtual_addr)];



    if (!entry_present(entry))
        return 0;



    return (entry_address(entry))
        | (virtual_addr & 0xFFF);
}





address_space_t* vmm_create_space(void)
{
    struct address_space* space =
        kmalloc(sizeof(struct address_space));


    if (space == NULL)
        return NULL;



    uint64_t* pml4 =
        allocate_table();



    if (pml4 == NULL)
    {
        return NULL;
    }



    /* Copy kernel mappings */
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
       Phase 1:
       - Page table freeing not implemented
       - Mapped page freeing not implemented
       - Space freeing not implemented
    */
}