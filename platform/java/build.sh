#!/bin/bash

# Notification Center Java 统一构建脚本
# 作者: DJI
# 版本: 1.0

# Exit immediately if a command exits with a non-zero status.
set -e

# 颜色定义
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 日志函数
log_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

log_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

log_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# --- Configuration ---
ROOT_DIR=$(git rev-parse --show-toplevel)
BUILD_DIR="$ROOT_DIR/build_java"
DIST_DIR="$ROOT_DIR/platform/java/dist"
PLATFORM_JAVA_DIR="$ROOT_DIR/platform/java"

# 检查前置条件
check_prerequisites() {
    log_info "检查构建前置条件..."
    
    # 检查CMake
    if ! command -v cmake &> /dev/null; then
        log_error "CMake未安装或不在PATH中"
        exit 1
    fi
    
    # 检查SWIG
    if ! command -v swig &> /dev/null; then
        log_error "SWIG未安装或不在PATH中"
        exit 1
    fi
    
    # 检查Java
    if ! command -v java &> /dev/null; then
        log_error "Java未安装或不在PATH中"
        exit 1
    fi
    
    # 检查Gradle
    if [ ! -f "$PLATFORM_JAVA_DIR/gradlew" ]; then
        log_error "Gradle Wrapper不存在: $PLATFORM_JAVA_DIR/gradlew"
        exit 1
    fi
    
    log_success "前置条件检查完成"
}

# 清理构建产物
clean() {
    log_info "清理构建产物..."
    
    # 清理Gradle构建产物
    cd "$PLATFORM_JAVA_DIR"
    ./gradlew clean
    cd "$ROOT_DIR"
    
    # 清理CMake构建产物
    rm -rf "$BUILD_DIR"
    rm -rf "$DIST_DIR"
    mkdir -p "$DIST_DIR"
    
    log_success "清理完成"
}

# 构建JNI库
build_jni() {
    log_info "构建JNI库..."
    
    # 清理之前的构建目录
    rm -rf "$BUILD_DIR"
    mkdir -p "$BUILD_DIR"
    
    # 配置CMake
    cmake -S "$ROOT_DIR" -B "$BUILD_DIR" -G "Unix Makefiles" -DUSE_LIBWEBSOCKETS=ON
    
    # 构建
    cmake --build "$BUILD_DIR" -- -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)
    
    # 查找并复制JNI库
    if [[ "$OSTYPE" == "darwin"* ]]; then
        JNI_LIB_NAME="libnotificationcenter.jnilib"
        JNI_LIB_PATTERN="*.jnilib"
    elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
        JNI_LIB_NAME="libnotificationcenter.so"
        JNI_LIB_PATTERN="*.so"
    elif [[ "$OSTYPE" == "msys" ]] || [[ "$OSTYPE" == "cygwin" ]]; then
        JNI_LIB_NAME="notificationcenter.dll"
        JNI_LIB_PATTERN="*.dll"
    else
        log_error "不支持的操作系统: $OSTYPE"
        exit 1
    fi
    
    # 查找JNI库文件
    JNI_LIB_PATH=$(find "$BUILD_DIR" -name "$JNI_LIB_NAME" 2>/dev/null | head -1)
    
    if [ -z "$JNI_LIB_PATH" ]; then
        JNI_LIB_PATH=$(find "$BUILD_DIR" -name "$JNI_LIB_PATTERN" 2>/dev/null | head -1)
    fi
    
    if [ -z "$JNI_LIB_PATH" ]; then
        log_error "无法找到JNI库文件"
        exit 1
    fi
    
    # 复制JNI库
    cp "$JNI_LIB_PATH" "$DIST_DIR/"
    
    log_success "JNI库构建完成"
}

# 构建SDK模块
build_sdk() {
    log_info "构建SDK模块..."
    cd "$PLATFORM_JAVA_DIR"
    ./gradlew :notification-center-sdk:build
    cd "$ROOT_DIR"
    log_success "SDK构建完成"
}

# 构建演示应用
build_demo() {
    log_info "构建演示应用..."
    cd "$PLATFORM_JAVA_DIR"
    ./gradlew :notification-center-demo:build
    cd "$ROOT_DIR"
    log_success "演示应用构建完成"
}

# 运行测试
run_tests() {
    log_info "运行测试..."
    cd "$PLATFORM_JAVA_DIR"
    ./gradlew test
    cd "$ROOT_DIR"
    log_success "测试完成"
}

# 运行演示
run_demo() {
    log_info "运行演示..."
    cd "$PLATFORM_JAVA_DIR"
    ./gradlew :notification-center-demo:run
    cd "$ROOT_DIR"
}

# 创建发布包
create_release() {
    log_info "创建发布包..."
    
    cd "$PLATFORM_JAVA_DIR"
    
    # 创建发布目录
    mkdir -p dist/
    
    # 复制SDK jar包
    cp notification-center-sdk/build/libs/*.jar dist/
    
    # 复制演示应用jar包（如果存在）
    if [ -d "notification-center-demo/build/libs" ]; then
        cp notification-center-demo/build/libs/*.jar dist/
    else
        log_info "演示应用使用application插件，不生成jar包"
    fi
    
    # 复制JNI库（如果存在）
    if [ -d "dist" ] && [ "$(ls -A dist/*.jnilib dist/*.so dist/*.dll dist/*.dylib 2>/dev/null)" ]; then
        log_info "JNI库已存在于发布目录中"
    else
        log_warning "未找到JNI库文件，请先运行 'jni' 命令构建JNI库"
    fi
    
    # 创建README
    cat > dist/README.txt << EOF
Notification Center Java SDK 发布包

包含文件：
- notification-center-sdk-*.jar: SDK核心包
- notification-center-sdk-*-fat.jar: 包含所有依赖的fat jar包
- *.jnilib/*.so/*.dll/*.dylib: JNI库文件

使用说明：
1. 将JNI库文件放在java.library.path中
2. 将SDK jar包添加到classpath
3. 参考演示应用代码使用SDK

版本: $(./gradlew properties | grep version | head -1 | awk '{print $2}')
构建时间: $(date)
EOF
    
    cd "$ROOT_DIR"
    log_success "发布包创建完成，位于 $PLATFORM_JAVA_DIR/dist/ 目录"
}

# 显示帮助信息
show_help() {
    echo "Notification Center Java 统一构建脚本"
    echo ""
    echo "用法: $0 [选项]"
    echo ""
    echo "选项:"
    echo "  clean        清理构建产物"
    echo "  jni          只构建JNI库"
    echo "  sdk          只构建SDK模块"
    echo "  demo         只构建演示应用"
    echo "  test         运行测试"
    echo "  run          运行演示应用"
    echo "  all          构建所有Java模块（不包含JNI）"
    echo "  full         完整构建（包含JNI）"
    echo "  release      创建发布包"
    echo "  help         显示此帮助信息"
    echo ""
    echo "示例:"
    echo "  $0 clean     清理项目"
    echo "  $0 jni       构建JNI库"
    echo "  $0 all       构建Java模块"
    echo "  $0 full      完整构建"
    echo "  $0 release   创建发布包"
}

# 主函数
main() {
    case "${1:-help}" in
        clean)
            clean
            ;;
        jni)
            check_prerequisites
            build_jni
            ;;
        sdk)
            check_prerequisites
            build_sdk
            ;;
        demo)
            check_prerequisites
            build_demo
            ;;
        test)
            check_prerequisites
            run_tests
            ;;
        run)
            check_prerequisites
            run_demo
            ;;
        all)
            check_prerequisites
            build_sdk
            build_demo
            run_tests
            ;;
        full)
            log_info "执行完整构建流程（包含JNI）..."
            check_prerequisites
            clean
            build_jni
            build_sdk
            build_demo
            run_tests
            ;;
        release)
            check_prerequisites
            build_sdk
            build_demo
            create_release
            ;;
        help|--help|-h)
            show_help
            ;;
        *)
            log_error "未知选项: $1"
            show_help
            exit 1
            ;;
    esac
}

# 执行主函数
main "$@" 