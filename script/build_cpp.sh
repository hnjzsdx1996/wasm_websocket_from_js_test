#!/bin/bash
set -e

# 工程根目录
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}" )/.." && pwd)"
SRC_DIR="$ROOT_DIR/src"
BUILD_DIR="$ROOT_DIR/build_cpp"

# 检查编译器
if ! command -v g++ >/dev/null 2>&1 && ! command -v clang++ >/dev/null 2>&1; then
  echo "请安装C++编译器 (g++ 或 clang++)"
  exit 1
fi

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# 配置和编译
cmake "$SRC_DIR"
cmake --build .

echo "C++编译完成，输出在 $BUILD_DIR/" 