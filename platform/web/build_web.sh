#!/bin/bash
set -e

# 加载 emsdk 环境
source ../../emsdk/emsdk_env.sh

# 工程根目录
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}" )/../.." && pwd)"
SRC_DIR="$ROOT_DIR/src"
BUILD_DIR="$ROOT_DIR/build"
WEB_DIR="$ROOT_DIR/platform/web/web_demo"
PLATFORM_WEB_DIR="$ROOT_DIR/platform/web"

# 检查 emsdk 环境
if ! command -v emcmake >/dev/null 2>&1; then
  echo "请先激活emsdk环境 (source ../../emsdk/emsdk_env.sh)"
  exit 1
fi

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# 配置和编译（使用 platform/web 的 CMakeLists.txt）
emcmake cmake "$PLATFORM_WEB_DIR"
cmake --build .

# 拷贝输出到 web_demo 目录，先清理旧文件
mkdir -p "$WEB_DIR"
rm -f "$WEB_DIR"/NotificationCenterSDK.js "$WEB_DIR"/NotificationCenterSDK.wasm
cp NotificationCenterSDK.js NotificationCenterSDK.wasm "$WEB_DIR/"

echo "编译完成，输出已拷贝到 $WEB_DIR/" 