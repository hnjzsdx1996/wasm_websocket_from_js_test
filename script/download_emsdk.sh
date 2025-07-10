#!/bin/bash
set -e

# 工程根目录
ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
EMSDK_DIR="$ROOT_DIR/emsdk"

# 如果emsdk目录不存在则创建
mkdir -p "$EMSDK_DIR"
cd "$EMSDK_DIR"

# 下载emsdk仓库
if [ ! -d ".git" ]; then
  echo "正在克隆 emsdk..."
  git clone https://github.com/emscripten-core/emsdk.git .
else
  echo "emsdk 已存在，拉取最新代码..."
  git pull
fi

# 安装和激活最新版本
./emsdk install latest
./emsdk activate latest

# 提示用户如何设置环境变量
cat <<EOF
\nemsdk 已下载并安装在 $EMSDK_DIR\n要设置环境变量，请执行：\n  source $EMSDK_DIR/emsdk_env.sh\nEOF 