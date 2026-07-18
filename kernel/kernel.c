void kernel_main(void)
{
    pmm_init(64 * 1024 * 1024);

    heap_init();

    thread_init();

    execution_init();

    scheduler_init();

    vmm_init();

    while (1)
    {
    }
}