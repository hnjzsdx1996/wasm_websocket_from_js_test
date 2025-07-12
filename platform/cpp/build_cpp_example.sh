#!/bin/bash
set -e

# 工程根目录
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}" )/../.." && pwd)"
EXAMPLE_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}" )" && pwd)"
BUILD_DIR="$EXAMPLE_DIR/build"
LIB_BUILD_DIR="$ROOT_DIR/build_cpp"

# 获取CPU核心数，用于并行编译
if command -v nproc >/dev/null 2>&1; then
    # Linux系统
    CPU_CORES=$(nproc)
elif command -v sysctl >/dev/null 2>&1; then
    # macOS系统
    CPU_CORES=$(sysctl -n hw.ncpu)
else
    # 默认使用4个核心
    CPU_CORES=4
fi

echo "检测到 $CPU_CORES 个CPU核心，将使用并行编译"

# 先编译静态库
echo "编译静态库..."
mkdir -p "$LIB_BUILD_DIR"
cd "$LIB_BUILD_DIR"
cmake "$ROOT_DIR/src"
cmake --build . --parallel $CPU_CORES

# 再编译 C++ 示例
echo "编译 C++ 示例..."
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"
cmake ..
cmake --build . --parallel $CPU_CORES

echo "C++ Example 编译完成，输出在 $BUILD_DIR/" 