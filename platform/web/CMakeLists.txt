cmake_minimum_required(VERSION 3.0)
project(wasm_websocket_sdk_web)

set(CMAKE_CXX_STANDARD 17)

# 包含src目录的头文件
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src/websocket)

# 源文件（包含src目录下的所有源文件）
set(SRC
    ../../src/websocket/JSWebSocket.cpp
    ../../src/SDKManager.cpp
    ../../src/websocket/CppWebSocket.cpp
    ../../src/websocket/WebSocketBase.h
    ../../src/websocket/JSWebSocket.h
    ../../src/SDKManager.h
    ../../src/websocket/CppWebSocket.h
    ../../src/websocket/WebSocketHolder.cpp
    ../../src/websocket/WebSocketHolder.h
    ../../src/c/sdk_c_api.cpp
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
        "SHELL:-sALLOW_TABLE_GROWTH=1"
        "SHELL:-sEXPORTED_RUNTIME_METHODS=['ccall','cwrap','intArrayFromString','UTF8ToString','stringToUTF8','lengthBytesUTF8','addFunction']"
        "SHELL:--bind"
        # 导出所有 C API 和 glue code 回调
        "SHELL:-sEXPORTED_FUNCTIONS=['_sdk_create','_sdk_destroy','_sdk_configure','_sdk_connect','_sdk_send','_sdk_close','_sdk_set_websocket','_sdk_set_message_callback','_sdk_set_open_callback','_sdk_set_close_callback','_sdk_set_error_callback','_malloc','_free','_js_websocket_on_open','_js_websocket_on_message','_js_websocket_on_close','_js_websocket_on_error']"
        # 用 --js-library 注入 glue code
        "SHELL:--js-library=${CMAKE_CURRENT_LIST_DIR}/websocket_glue.js"
    )
endif() 