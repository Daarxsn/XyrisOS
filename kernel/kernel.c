void kernel_main(void)
{
    memory_map_init();

    memory_map_dump();

    hhdm_init();

    pmm_init();

    heap_init();

    thread_init();

    execution_init();

    scheduler_init();

    vmm_init();

    for (;;)
        __asm__ volatile ("hlt");
}