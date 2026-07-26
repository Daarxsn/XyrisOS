#!/bin/bash

set -e

echo "=========================================="
echo "        XyrisOS Build System"
echo "=========================================="
echo ""

echo "[1/2] Configuring XyrisOS..."

if [ ! -f "toolchain/x86_64-toolchain.cmake" ]; then
    echo "Error: Toolchain file not found."
    echo "Expected: toolchain/x86_64-toolchain.cmake"
    exit 1
fi

cmake -S . \
      -B build \
      -G Ninja \
      -DCMAKE_TOOLCHAIN_FILE=toolchain/x86_64-toolchain.cmake

echo ""
echo "[2/2] Building XyrisOS..."

cmake --build build

echo ""
echo "=========================================="
echo "   XyrisOS build completed successfully!"
echo "=========================================="