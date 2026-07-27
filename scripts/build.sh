#!/bin/bash

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

cd "$PROJECT_ROOT"

echo "=========================================="
echo "        XyrisOS Build System"
echo "=========================================="
echo ""

echo "[1/2] Configuring XyrisOS..."

if [ ! -f "$PROJECT_ROOT/toolchain/x86_64-toolchain.cmake" ]; then
    echo "Error: Toolchain file not found."
    echo "Expected: $PROJECT_ROOT/toolchain/x86_64-toolchain.cmake"
    exit 1
fi

cmake -S "$PROJECT_ROOT" \
      -B "$PROJECT_ROOT/build" \
      -G Ninja \
      -DCMAKE_TOOLCHAIN_FILE="$PROJECT_ROOT/toolchain/x86_64-toolchain.cmake"

echo ""
echo "[2/2] Building XyrisOS..."

cmake --build "$PROJECT_ROOT/build"

echo ""
echo "=========================================="
echo "   XyrisOS build completed successfully!"
echo "=========================================="