#!/bin/bash

set -e

echo "=========================================="
echo "         XyrisOS Development"
echo "=========================================="

echo ""
echo "[1/3] Building..."
./scripts/build.sh

echo ""
echo "[2/3] Creating ISO..."
./scripts/iso.sh

echo ""
echo "[3/3] Running..."
./scripts/run.sh