#!/bin/bash

set -e

echo "=========================================="
echo "      XyrisOS Simulator Test Suite"
echo "=========================================="

echo ""
echo "[1/3] Configuring simulator..."

cmake -S simulator \
      -B simulator/build \
      -G Ninja

echo ""
echo "[2/3] Building simulator..."

cmake --build simulator/build

echo ""
echo "[3/3] Running tests..."

ctest --test-dir simulator/build --output-on-failure

echo ""
echo "=========================================="
echo "   Simulator tests passed successfully!"
echo "=========================================="