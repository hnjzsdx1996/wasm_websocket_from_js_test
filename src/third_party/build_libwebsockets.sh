#!/bin/bash

set -e

# 进入脚本所在目录
cd "$(dirname "$0")/libwebsockets"

# 检测操作系统
if [[ "$OSTYPE" == "darwin"* ]]; then
    # macOS
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
    OPENSSL_CMAKE_ARGS="-DOPENSSL_ROOT_DIR=$OPENSSL_ROOT -DOPENSSL_INCLUDE_DIR=$OPENSSL_ROOT/include"
    
    # macOS 获取 CPU 核心数
    CPU_CORES=$(sysctl -n hw.ncpu 2>/dev/null || echo 4)
    
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    # Linux
    echo "检测到 Linux 系统"
    
    # 检查是否安装了 OpenSSL 开发包
    if ! pkg-config --exists openssl; then
        echo "未找到 OpenSSL 开发包，请安装:"
        echo "Ubuntu/Debian: sudo apt-get install libssl-dev"
        echo "CentOS/RHEL: sudo yum install openssl-devel"
        echo "Fedora: sudo dnf install openssl-devel"
        exit 1
    fi
    
    # 获取 OpenSSL 路径
    OPENSSL_INCLUDE_DIR=$(pkg-config --cflags-only-I openssl | sed 's/-I//')
    OPENSSL_LIBRARY_DIR=$(pkg-config --libs-only-L openssl | sed 's/-L//')
    
    echo "OpenSSL include dir: $OPENSSL_INCLUDE_DIR"
    echo "OpenSSL library dir: $OPENSSL_LIBRARY_DIR"
    
    OPENSSL_CMAKE_ARGS="-DOPENSSL_INCLUDE_DIR=$OPENSSL_INCLUDE_DIR -DOPENSSL_LIBRARY_DIR=$OPENSSL_LIBRARY_DIR"
    
    # Linux 获取 CPU 核心数
    CPU_CORES=$(nproc 2>/dev/null || grep -c processor /proc/cpuinfo 2>/dev/null || echo 4)
    
else
    echo "不支持的操作系统: $OSTYPE"
    exit 1
fi

# 创建 build 目录
mkdir -p build
cd build

# 配置 CMake
cmake .. \
    $OPENSSL_CMAKE_ARGS \
    -DCMAKE_BUILD_TYPE=Release \
    -DLWS_WITH_STATIC=ON \
    -DLWS_WITH_SHARED=ON \
    -DLWS_WITH_MINIMAL_EXAMPLES=OFF \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON

# 编译
echo "使用 $CPU_CORES 核心进行编译"
make -j$CPU_CORES

echo "libwebsockets 编译完成！" 