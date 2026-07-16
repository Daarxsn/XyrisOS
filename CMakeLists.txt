cmake_minimum_required(VERSION 3.20)

project(XyrisOS C ASM)

enable_language(ASM)

set(CMAKE_C_STANDARD 23)

add_executable(kernel.elf
    kernel/core/kernel.c
    kernel/graphics/framebuffer.c
    kernel/graphics/graphics.c
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