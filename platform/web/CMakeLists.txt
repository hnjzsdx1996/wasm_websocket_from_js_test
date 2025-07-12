cmake_minimum_required(VERSION 3.0)
project(wasm_websocket_sdk_web)

set(CMAKE_CXX_STANDARD 17)

# 直接编译所有需要的源文件，不依赖 src 目录的静态库
set(ALL_SOURCES
    # 主库源文件
    ${CMAKE_CURRENT_LIST_DIR}/../../src/base/async/thread_pool_executor.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../../src/SDKManager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../../src/websocket/CppWebSocket.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../../src/websocket/WebSocketHolder.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../../src/c/sdk_c_api.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../../src/base/logger/thread_info.cpp
    # Web 平台特定源文件
    src/JSWebSocket.cpp
    src/sdk_c_api_web.cpp
)

add_executable(NotificationCenterSDKWeb ${ALL_SOURCES})

# 设置编译选项
target_compile_options(NotificationCenterSDKWeb PRIVATE
    $<$<BOOL:${EMSCRIPTEN}>:-pthread>
    $<$<BOOL:${EMSCRIPTEN}>:-matomics>
    $<$<BOOL:${EMSCRIPTEN}>:-mbulk-memory>
)

# 包含 third_party 配置
include(${CMAKE_CURRENT_LIST_DIR}/../../src/third_party/ThirdParty.cmake)

# 设置目标特定的头文件路径
target_include_directories(NotificationCenterSDKWeb PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/../../src
    ${CMAKE_CURRENT_LIST_DIR}/../../src/websocket
    ${CMAKE_CURRENT_LIST_DIR}/../../src/base/logger
    ${CMAKE_CURRENT_LIST_DIR}/../../src/third_party/plog/include
)

# emscripten专用设置
if(EMSCRIPTEN)
    set_target_properties(NotificationCenterSDKWeb PROPERTIES
        OUTPUT_NAME "NotificationCenterSDK"
        SUFFIX ".js"
    )
    target_link_options(NotificationCenterSDKWeb PRIVATE
        "SHELL:-sMODULARIZE=1"
        "SHELL:-sEXPORT_NAME=NotificationCenterSDK"
        "SHELL:-sINITIAL_MEMORY=32MB"
        "SHELL:-sMAXIMUM_MEMORY=512MB"
        "SHELL:-sALLOW_TABLE_GROWTH=1"
        "SHELL:-sEXPORTED_RUNTIME_METHODS=['ccall','cwrap','intArrayFromString','UTF8ToString','stringToUTF8','lengthBytesUTF8','addFunction']"
        "SHELL:--bind"
        # 多线程支持
        "SHELL:-pthread"
        "SHELL:-sUSE_PTHREADS=1"
        "SHELL:-sPTHREAD_POOL_SIZE=4"
        "SHELL:-matomics"
        "SHELL:-mbulk-memory"
        # 导出所有 C API 和 glue code 回调
        "SHELL:-sEXPORTED_FUNCTIONS=['_js_sdk_create','_js_sdk_destroy','_js_sdk_configure','_js_sdk_connect','_js_sdk_send','_js_sdk_close','_js_sdk_set_websocket','_js_sdk_set_message_callback','_js_sdk_set_open_callback','_js_sdk_set_close_callback','_js_sdk_set_error_callback','_js_sdk_create_js_websocket','_malloc','_free','_js_websocket_on_open','_js_websocket_on_message','_js_websocket_on_close','_js_websocket_on_error']"
        # 用 --js-library 注入 glue code
        "SHELL:--js-library=${CMAKE_CURRENT_LIST_DIR}/js_glue/NotificationCenterSDK_glue.js"
    )
endif() 