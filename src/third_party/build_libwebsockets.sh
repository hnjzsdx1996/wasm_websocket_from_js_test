#!/bin/bash

set -e

# 进入脚本所在目录
cd "$(dirname "$0")/libwebsockets"

# 检查 openssl 路径
if command -v brew >/dev/null 2>&1; then
    OPENSSL_ROOT=$(brew --prefix openssl@3 2>/dev/null || brew --prefix openssl 2>/dev/null)
else
    echo "请先安装 Homebrew 和 openssl"
    exit 1
fi

if [ ! -d "$OPENSSL_ROOT" ]; then
    echo "未找到 openssl，请先执行: brew install openssl"
    exit 1
fi

echo "使用 OpenSSL 路径: $OPENSSL_ROOT"

# 创建 build 目录
mkdir -p build
cd build

# 配置 CMake
cmake .. \
    -DOPENSSL_ROOT_DIR="$OPENSSL_ROOT" \
    -DOPENSSL_INCLUDE_DIR="$OPENSSL_ROOT/include" \
    -DCMAKE_BUILD_TYPE=Release \
    -DLWS_WITH_STATIC=ON \
    -DLWS_WITH_SHARED=ON \
    -DLWS_WITH_MINIMAL_EXAMPLES=OFF

# 编译
CPU_CORES=$(sysctl -n hw.ncpu)
echo "使用 $CPU_CORES 核心进行编译"
make -j$CPU_CORES

echo "libwebsockets 编译完成！" 