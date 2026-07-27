#!/bin/bash

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

echo "=========================================="
echo "         XyrisOS Development"
echo "=========================================="

echo ""
echo "[1/3] Building..."
"$PROJECT_ROOT/scripts/build.sh"

echo ""
echo "[2/3] Creating ISO..."
"$PROJECT_ROOT/scripts/iso.sh"

echo ""
echo "[3/3] Running..."
"$PROJECT_ROOT/scripts/run.sh"