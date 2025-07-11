cmake_minimum_required(VERSION 3.0)
project(wasm_websocket_sdk)

set(CMAKE_CXX_STANDARD 17)

# 源文件
set(SOURCES
    SDKManager.cpp
    websocket/CppWebSocket.cpp
    websocket/WebSocketHolder.cpp
    c/sdk_c_api.cpp
)

# 头文件目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/websocket)

# 使用 third_party
include(${CMAKE_CURRENT_SOURCE_DIR}/third_party/ThirdParty.cmake)

# 生成可执行文件
add_library(NotificationCenterSDK STATIC ${SOURCES})
add_library(NotificationCenterSDKShared SHARED ${SOURCES})

# 纯C++编译设置
set_target_properties(NotificationCenterSDK PROPERTIES
    OUTPUT_NAME "NotificationCenterSDK"
) 