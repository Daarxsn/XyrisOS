cmake_minimum_required(VERSION 3.20)

project(XyrisOS C ASM)

enable_language(ASM)

set(CMAKE_C_STANDARD 23)

add_executable(kernel.elf

    # Core
    kernel/core/kernel.c

    # Foundation
    kernel/foundation2/ukom.c
    kernel/foundation2/capability.c
    kernel/foundation2/resource.c
    kernel/foundation2/event.c
    kernel/foundation2/time.c
    kernel/foundation2/config.c

    # Graphics
    kernel/graphics/framebuffer.c
    kernel/graphics/graphics.c
    kernel/graphics/font.c

    # Terminal
    kernel/terminal/terminal.c

    # UI
    kernel/ui/ui.c

    # Images
    kernel/image/image.c
    kernel/image/logo.c

    # Memory
    kernel/memory/pmm.c
    kernel/memory/heap.c
    kernel/memory/vmm.c
    kernel/memory/bitmap.c
    kernel/memory/hhdm.c

    # Execution
    kernel/execution/thread.c
    kernel/execution/execution.c
    kernel/execution/scheduler.c

    # Logger
    kernel/logger/logger.c

    # CPU
    kernel/cpu/gdt.c
    kernel/cpu/idt.c
    kernel/cpu/isr.c
    kernel/cpu/exceptions.c
    kernel/cpu/interrupt.S
    kernel/cpu/io.c
    kernel/cpu/pic.c
    kernel/cpu/irq.c
    kernel/cpu/pit.c
    kernel/cpu/cpu.c

    # Debug
    kernel/debug/panic.c
    kernel/debug/print.c
    kernel/debug/hex.c

    # Boot
    kernel/boot/boot.c
)

target_include_directories(kernel.elf PRIVATE
    kernel
    kernel/include
)

target_compile_options(kernel.elf PRIVATE
    -ffreestanding
    -fno-stack-protector
    -fno-stack-check
    -fno-lto
    -fno-pic
    -fno-pie
    -ffunction-sections
    -fdata-sections
    -m64
    -march=x86-64
    -mabi=sysv
    -mno-80387
    -mno-mmx
    -mno-sse
    -mno-sse2
    -mno-red-zone
    -mcmodel=kernel
    -Wall
    -Wextra
)

target_link_options(kernel.elf PRIVATE
    -nostdlib
    -static
    -Wl,-m,elf_x86_64
    -Wl,-z,max-page-size=0x1000
    -Wl,--gc-sections
    -T ${CMAKE_SOURCE_DIR}/boot/linker.ld
)