#!/bin/bash
# ──────────────────────────────────────────────────────────────────
# build.sh — Local build & test helper
# Usage:
#   ./scripts/build.sh            → Release build + tests
#   ./scripts/build.sh debug      → Debug build + tests
#   ./scripts/build.sh coverage   → Coverage build + lcov report
#   ./scripts/build.sh clean      → Remove build directory
# ──────────────────────────────────────────────────────────────────

set -euo pipefail

MODE="${1:-release}"
BUILD_DIR="build"

clean() {
    echo "[build.sh] Cleaning build directory..."
    rm -rf "$BUILD_DIR"
}

build_release() {
    echo "[build.sh] Configuring Release build..."
    cmake -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Release -DENABLE_TESTING=ON
    cmake --build "$BUILD_DIR" --parallel
    run_tests
}

build_debug() {
    echo "[build.sh] Configuring Debug build..."
    cmake -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Debug -DENABLE_TESTING=ON
    cmake --build "$BUILD_DIR" --parallel
    run_tests
}

build_coverage() {
    BUILD_DIR="build-cov"
    echo "[build.sh] Configuring Coverage build..."
    cmake -B "$BUILD_DIR" \
        -DCMAKE_BUILD_TYPE=Debug \
        -DENABLE_TESTING=ON \
        -DENABLE_COVERAGE=ON
    cmake --build "$BUILD_DIR" --parallel
    run_tests

    echo "[build.sh] Generating coverage report..."
    lcov --capture --directory "$BUILD_DIR" --output-file coverage.info --ignore-errors mismatch
    lcov --remove coverage.info '/usr/*' '*/googletest/*' '*/tests/*' --output-file coverage.info
    lcov --list coverage.info
    echo "[build.sh] Coverage report: coverage.info"
}

run_tests() {
    echo "[build.sh] Running tests..."
    cd "$BUILD_DIR"
    ctest --output-on-failure
    cd ..
    echo "[build.sh] All tests passed ✓"
}

case "$MODE" in
    clean)    clean ;;
    debug)    build_debug ;;
    coverage) build_coverage ;;
    release)  build_release ;;
    *)
        echo "Unknown mode: $MODE"
        echo "Usage: $0 [release|debug|coverage|clean]"
        exit 1
        ;;
esac
