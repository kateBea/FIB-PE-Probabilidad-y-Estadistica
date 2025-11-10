#!/usr/bin/env bash
set -e  # stop on first error

# ======================================
# Configuration
# ======================================
BUILD_DIR="build"
GENERATOR_DIR="$BUILD_DIR/generator"
TESTER_DIR="$BUILD_DIR/tester"
GENERATOR_BIN="./generator"  # relative to GENERATOR_DIR
TESTER_BIN="./tester"        # relative to TESTER_DIR

# ======================================
# Build
# ======================================
echo "Building project..."
cmake -S . -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Debug
cmake --build "$BUILD_DIR" --parallel

# ======================================
# Run generator
# ======================================
if [ -x "$GENERATOR_DIR/$GENERATOR_BIN" ]; then
    echo "Running generator..."
    (cd "$GENERATOR_DIR" && "$GENERATOR_BIN")
else
    echo "Generator binary not found at: $GENERATOR_DIR/$GENERATOR_BIN"
    exit 1
fi

# ======================================
# Run tester
# ======================================
if [ -x "$TESTER_DIR/$TESTER_BIN" ]; then
    echo "Running tester..."
    (cd "$TESTER_DIR" && "$TESTER_BIN")
else
    echo "Tester binary not found at: $TESTER_DIR/$TESTER_BIN"
    exit 1
fi

echo "All done!"
