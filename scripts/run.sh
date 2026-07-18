#!/bin/bash

set -e

echo "====================================="
echo "        Starting XyrisOS"
echo "====================================="

qemu-system-x86_64 \
    -m 512M \
    -cdrom XyrisOS.iso \
    -boot d \
    -serial stdio \
    -no-reboot
