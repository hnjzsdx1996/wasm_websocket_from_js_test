cmake_minimum_required(VERSION 3.0)
project(wasm_websocket_sdk)

set(CMAKE_CXX_STANDARD 17)

# 源文件
set(SRC
    JSWebSocket.cpp
    SDKManager.cpp
)

# 头文件目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# 生成wasm和js
add_executable(NotificationCenterSDK ${SRC})

# emscripten专用设置
if(EMSCRIPTEN)
    set_target_properties(NotificationCenterSDK PROPERTIES
        OUTPUT_NAME "NotificationCenterSDK"
        SUFFIX ".js"
    )
    target_link_options(NotificationCenterSDK PRIVATE
        "SHELL:-sMODULARIZE=1"
        "SHELL:-sEXPORT_NAME=NotificationCenterSDK"
        "SHELL:-sALLOW_MEMORY_GROWTH=1"
        "SHELL:-sEXPORTED_RUNTIME_METHODS=['ccall','cwrap','allocate','intArrayFromString','ALLOC_STACK']"
        "SHELL:--bind"
    )
endif() 