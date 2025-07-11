cmake_minimum_required(VERSION 3.0)
project(wasm_websocket_sdk_web)

set(CMAKE_CXX_STANDARD 17)

# 包含src目录的头文件
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src)

# 源文件（包含src目录下的所有源文件）
set(SRC
    ../../src/JSWebSocket.cpp
    ../../src/SDKManager.cpp
    ../../src/CppWebSocket.cpp
    ../../src/WebSocketBase.h
    ../../src/JSWebSocket.h
    ../../src/SDKManager.h
    ../../src/CppWebSocket.h
)

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
        "SHELL:-sEXPORTED_RUNTIME_METHODS=['ccall','cwrap','intArrayFromString','UTF8ToString','stringToUTF8','lengthBytesUTF8']"
        "SHELL:--bind"
        # 只导出 C++ 需要 JS 调用的回调
        "SHELL:-sEXPORTED_FUNCTIONS=['_malloc','_free','_js_websocket_on_open','_js_websocket_on_message','_js_websocket_on_close','_js_websocket_on_error']"
        # 用 --js-library 注入 glue code
        "SHELL:--js-library=${CMAKE_CURRENT_LIST_DIR}/websocket_glue.js"
    )
endif() 