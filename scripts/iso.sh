#!/bin/bash

set -e

echo "========================================="
echo "      Creating XyrisOS Bootable ISO"
echo "========================================="

rm -rf iso_root
mkdir -p iso_root/boot/limine
mkdir -p iso_root/EFI/BOOT

echo "[1/5] Copying kernel..."
cp build/kernel.elf iso_root/boot/kernel.elf

echo "[2/5] Copying Limine configuration..."
cp boot/limine/limine.conf iso_root/boot/limine/

echo "[3/5] Copying Limine binaries..."
cp limine-binary/limine-bios.sys iso_root/boot/limine/
cp limine-binary/limine-bios-cd.bin iso_root/boot/limine/
cp limine-binary/limine-uefi-cd.bin iso_root/boot/limine/

cp limine-binary/BOOTX64.EFI iso_root/EFI/BOOT/
cp limine-binary/BOOTIA32.EFI iso_root/EFI/BOOT/

echo "[4/5] Creating ISO..."

xorriso -as mkisofs \
    -R \
    -r \
    -J \
    -b boot/limine/limine-bios-cd.bin \
    -no-emul-boot \
    -boot-load-size 4 \
    -boot-info-table \
    -hfsplus \
    -apm-block-size 2048 \
    --efi-boot boot/limine/limine-uefi-cd.bin \
    -efi-boot-part \
    --efi-boot-image \
    --protective-msdos-label \
    iso_root \
    -o XyrisOS.iso

echo "[5/5] Installing Limine..."

./limine-binary/limine bios-install XyrisOS.iso

echo ""
echo "========================================="
echo "ISO Created Successfully!"
echo "========================================="