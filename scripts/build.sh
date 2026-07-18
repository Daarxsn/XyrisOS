#!/bin/bash

set -e

echo "=========================================="
echo "        XyrisOS Build System"
echo "=========================================="

cmake -S . \
      -B build \
      -G Ninja \
      -DCMAKE_TOOLCHAIN_FILE=toolchain/x86_64-toolchain.cmake

cmake --build build

echo ""
echo "Build completed successfully."