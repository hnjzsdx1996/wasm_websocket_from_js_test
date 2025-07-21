#!/bin/bash
set -e

# 工程根目录
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}" )/.." && pwd)"
SRC_DIR="$ROOT_DIR/src"
BUILD_DIR="$ROOT_DIR/build_cpp"

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

# 检查编译器
if ! command -v g++ >/dev/null 2>&1 && ! command -v clang++ >/dev/null 2>&1; then
  echo "请安装C++编译器 (g++ 或 clang++)"
  exit 1
fi

echo "检测到 $CPU_CORES 个CPU核心，将使用并行编译"

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# 配置和编译
echo "配置CMake..."
cmake "$SRC_DIR" -DUSE_LIBWEBSOCKETS=ON

echo "开始并行编译 (使用 $CPU_CORES 个核心)..."
cmake --build . --parallel $CPU_CORES

echo "C++编译完成，输出在 $BUILD_DIR/" 