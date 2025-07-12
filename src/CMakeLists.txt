cmake_minimum_required(VERSION 3.0)
project(NotificationCenterSDK)

set(CMAKE_CXX_STANDARD 17)

# 自动收集所有源文件
file(GLOB_RECURSE SOURCES 
    "*.cpp"
    "*.c"
)

# 排除第三方库文件（如果有的话）
list(FILTER SOURCES EXCLUDE REGEX "third_party/.*")

# 使用 third_party
include(${CMAKE_CURRENT_SOURCE_DIR}/third_party/ThirdParty.cmake)

# 生成库
add_library(NotificationCenterSDK STATIC ${SOURCES})
add_library(NotificationCenterSDKShared SHARED ${SOURCES})

# 设置目标属性
set_target_properties(NotificationCenterSDK PROPERTIES
    OUTPUT_NAME "NotificationCenterSDK"
)

set_target_properties(NotificationCenterSDKShared PROPERTIES
    OUTPUT_NAME "NotificationCenterSDK"
)

# 使用现代的 target_include_directories
target_include_directories(NotificationCenterSDK PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/websocket>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/base/logger>
)

target_include_directories(NotificationCenterSDKShared PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/websocket>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/base/logger>
) 