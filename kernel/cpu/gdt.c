#include "gdt.h"

static gdt_entry_t gdt[3];
static gdt_ptr_t gdt_ptr;

static void gdt_set_entry(
    int index,
    uint32_t base,
    uint32_t limit,
    uint8_t access,
    uint8_t granularity)
{
    gdt[index].base_low = base & 0xFFFF;
    gdt[index].base_mid = (base >> 16) & 0xFF;
    gdt[index].base_high = (base >> 24) & 0xFF;

    gdt[index].limit_low = limit & 0xFFFF;

    gdt[index].granularity =
        ((limit >> 16) & 0x0F);

    gdt[index].granularity |=
        granularity & 0xF0;

    gdt[index].access = access;
}

static inline void gdt_load(gdt_ptr_t *ptr)
{
    __asm__ volatile (
        "lgdt (%0)"
        :
        : "r"(ptr)
        : "memory"
    );

    __asm__ volatile (
        "mov $0x10, %%ax\n\t"
        "mov %%ax, %%ds\n\t"
        "mov %%ax, %%es\n\t"
        "mov %%ax, %%fs\n\t"
        "mov %%ax, %%gs\n\t"
        "mov %%ax, %%ss\n\t"
        :
        :
        : "ax", "memory"
    );
}

void gdt_init(void)
{
    gdt_ptr.limit = sizeof(gdt) - 1;
    gdt_ptr.base = (uint64_t)&gdt;

    gdt_set_entry(0, 0, 0, 0, 0);

    gdt_set_entry(
        1,
        0,
        0xFFFFF,
        0x9A,
        0xA0
    );

    gdt_set_entry(
        2,
        0,
        0xFFFFF,
        0x92,
        0xA0
    );

    gdt_load(&gdt_ptr);
}