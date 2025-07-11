cmake_minimum_required(VERSION 3.0)
project(wasm_websocket_sdk_web)

set(CMAKE_CXX_STANDARD 17)

# 包含src目录和web/src目录的头文件
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src/websocket)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# 源文件（web相关和平台无关的都要）
set(SRC
    src/JSWebSocket.cpp
    src/sdk_c_api_web.cpp
    ../../src/SDKManager.cpp
    ../../src/websocket/CppWebSocket.cpp
    ../../src/websocket/WebSocketBase.h
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
        "SHELL:-sEXPORTED_FUNCTIONS=['_js_sdk_create','_js_sdk_destroy','_js_sdk_configure','_js_sdk_connect','_js_sdk_send','_js_sdk_close','_js_sdk_set_websocket','_js_sdk_set_message_callback','_js_sdk_set_open_callback','_js_sdk_set_close_callback','_js_sdk_set_error_callback','_js_sdk_create_js_websocket','_malloc','_free','_js_websocket_on_open','_js_websocket_on_message','_js_websocket_on_close','_js_websocket_on_error']"
        # 用 --js-library 注入 glue code
        "SHELL:--js-library=${CMAKE_CURRENT_LIST_DIR}/js_glue/NotificationCenterSDK_glue.js"
    )
endif() 