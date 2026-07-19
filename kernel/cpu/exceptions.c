#include "exceptions.h"
#include "../debug/panic.h"

static const char *exception_names[32] =
{
    "Divide Error",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "BOUND Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved",
    "x87 Floating Point",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating Point",
    "Virtualization",
    "Control Protection",
    "Reserved","Reserved","Reserved","Reserved",
    "Reserved","Reserved","Hypervisor Injection",
    "VMM Communication",
    "Security Exception",
    "Reserved"
};

void exception_dispatch(registers_t *regs)
{
    (void)regs;

    const char *name = "Unknown Exception";

    if (regs->vector < 32)
        name = exception_names[regs->vector];

    (void)name;

    for (;;)
        kernel_panic(regs, name);
}