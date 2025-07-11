cmake_minimum_required(VERSION 3.0)
project(NotifycationCenterSDKAll)

# 主库
add_subdirectory(src)

# C++ 示例
add_subdirectory(platform/cpp)

# Web 平台（如需 emscripten 构建，可取消注释）
# add_subdirectory(platform/web)

