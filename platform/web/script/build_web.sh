#!/bin/bash
set -e

# 加载 emsdk 环境
source ../emsdk/emsdk_env.sh

# 工程根目录
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}" )/../../.." && pwd)"
SRC_DIR="$ROOT_DIR/src"
BUILD_DIR="$ROOT_DIR/platform/web/build"
WEB_DIR="$ROOT_DIR/platform/web/dist"
WEB_DEMO_DIR="$ROOT_DIR/platform/web/web_demo"
PLATFORM_WEB_DIR="$ROOT_DIR/platform/web"

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

# 检查 emsdk 环境
if ! command -v emcmake >/dev/null 2>&1; then
  echo "激活emsdk环境失败"
  exit 1
fi

echo "检测到 $CPU_CORES 个CPU核心，将使用并行编译"

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# 配置和编译（使用 platform/web 的 CMakeLists.txt）
echo "配置CMake..."
emcmake cmake "$PLATFORM_WEB_DIR"

echo "开始并行编译 (使用 $CPU_CORES 个核心)..."
cmake --build . --parallel $CPU_CORES

# 拷贝输出到 web_demo 目录，先清理旧文件
echo "复制输出文件..."
mkdir -p "$WEB_DIR"
rm -f "$WEB_DIR"/NotificationCenterSDK.js "$WEB_DIR"/NotificationCenterSDK.wasm
cp NotificationCenterSDK.js NotificationCenterSDK.wasm "$WEB_DIR/"

mkdir -p "$WEB_DEMO_DIR"
rm -f "$WEB_DEMO_DIR"/NotificationCenterSDK.js "$WEB_DEMO_DIR"/NotificationCenterSDK.wasm
cp NotificationCenterSDK.js NotificationCenterSDK.wasm "$WEB_DEMO_DIR/"

echo "编译完成" 