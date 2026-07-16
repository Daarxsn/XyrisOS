cmake_minimum_required(VERSION 3.20)

project(XyrisOS C ASM)

enable_language(ASM)

set(CMAKE_C_STANDARD 23)

# -------------------------------------------------
# Kernel Object Files
# -------------------------------------------------

add_library(kernel_objects OBJECT
    kernel/core/kernel.c
)

target_include_directories(kernel_objects PRIVATE
    kernel
    kernel/include
)

target_compile_options(kernel_objects PRIVATE
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

# -------------------------------------------------
# Link Kernel
# -------------------------------------------------

add_custom_command(
    OUTPUT kernel.elf

    COMMAND ld
        -m elf_x86_64
        -nostdlib
        -static
        -z max-page-size=0x1000
        --gc-sections
        -T ${CMAKE_SOURCE_DIR}/boot/linker.ld
        -o kernel.elf
        $<TARGET_OBJECTS:kernel_objects>

    DEPENDS kernel_objects
)

add_custom_target(kernel ALL
    DEPENDS kernel.elf
)