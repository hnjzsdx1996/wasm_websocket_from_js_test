#!/bin/bash
set -e

# 工程根目录
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}" )/../.." && pwd)"
EXAMPLE_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}" )" && pwd)"
BUILD_DIR="$EXAMPLE_DIR/build"

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

cmake ..
cmake --build .

echo "C++ Example 编译完成，输出在 $BUILD_DIR/" 