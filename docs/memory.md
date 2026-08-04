# XyrisOS Memory Management

## Overview

The XyrisOS memory subsystem provides physical memory management,
virtual memory management, kernel heap allocation, and memory testing.

## Physical Memory Manager (PMM)

The PMM manages physical pages using a bitmap-based allocator.

Features:
- Page allocation
- Multi-page allocation
- Page freeing
- Memory statistics tracking

## Higher Half Direct Map (HHDM)

HHDM provides a direct mapping between physical memory and virtual addresses,
allowing the kernel to access physical memory efficiently.

## Virtual Memory Manager (VMM)

The VMM manages x86_64 virtual address spaces using four-level paging.

Features:
- Page table walking
- Virtual address mapping
- Page translation
- Memory protection flags

## Kernel Heap

The kernel heap provides dynamic memory allocation.

Functions:
- kmalloc()
- kcalloc()
- krealloc()
- kfree()

The allocator currently uses a bump allocation strategy with 16-byte alignment.

## Testing

Memory subsystem tests include:

- PMM allocation tests
- PMM statistics tests
- Multi-page allocation tests
- Heap allocation tests
- Heap alignment verification
- Virtual memory mapping tests
- Page fault diagnostics

## Status

Memory subsystem implementation is complete for the current XyrisOS phase.