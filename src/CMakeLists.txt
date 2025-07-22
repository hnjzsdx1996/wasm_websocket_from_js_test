cmake_minimum_required(VERSION 3.0)
project(NotificationCenterSDK)

set(CMAKE_CXX_STANDARD 17)

# 自动收集所有源文件
file(GLOB_RECURSE SOURCES 
    "*.cpp"
    "*.c"
    websocket/WebSocketHolder.cpp
    websocket/WebsocketEvent.cpp
)

# 排除第三方库文件（如果有的话）
list(FILTER SOURCES EXCLUDE REGEX "third_party/.*")

# 使用 third_party
include(${CMAKE_CURRENT_SOURCE_DIR}/third_party/ThirdParty.cmake)

# 由外部传入 USE_LIBWEBSOCKETS 宏，不在 CMakeLists.txt 内部定义
if(DEFINED USE_LIBWEBSOCKETS AND USE_LIBWEBSOCKETS)
    # 查找 OpenSSL
    find_package(OpenSSL REQUIRED)
    add_definitions(-DENABLE_LIBWEBSOCKETS)
    
    # 确保找到 OpenSSL 库
    if(NOT OPENSSL_FOUND)
        message(FATAL_ERROR "OpenSSL not found")
    endif()
    
    message(STATUS "OpenSSL found: ${OPENSSL_VERSION}")
    message(STATUS "OpenSSL libraries: ${OPENSSL_LIBRARIES}")
    message(STATUS "OpenSSL include dir: ${OPENSSL_INCLUDE_DIR}")
endif()

# 生成库
add_library(NotificationCenterSDK STATIC ${SOURCES})
add_library(NotificationCenterSDKShared SHARED ${SOURCES})

# 为静态库设置 PIC 标志，以便可以链接到共享库
set_target_properties(NotificationCenterSDK PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

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
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party/plog/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party/AIGCJson/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party/AIGCJson/include/rapidjson>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party/AIGCJson/include/rapidjson/error>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party/AIGCJson/include/rapidjson/internal>
)

target_include_directories(NotificationCenterSDKShared PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/websocket>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/base/logger>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party/plog/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party/AIGCJson/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party/AIGCJson/include/rapidjson>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party/AIGCJson/include/rapidjson/error>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/third_party/AIGCJson/include/rapidjson/internal>
)

if(DEFINED USE_LIBWEBSOCKETS AND USE_LIBWEBSOCKETS)
    target_include_directories(NotificationCenterSDK PUBLIC ${OPENSSL_INCLUDE_DIR})
endif()

if(DEFINED USE_LIBWEBSOCKETS AND USE_LIBWEBSOCKETS)
    target_include_directories(NotificationCenterSDKShared PUBLIC ${OPENSSL_INCLUDE_DIR})
endif()

# 查找 pthread 库
find_package(Threads REQUIRED)

# 仅在启用 libwebsockets 时链接
if(DEFINED USE_LIBWEBSOCKETS AND USE_LIBWEBSOCKETS)
    # 链接顺序很重要：先链接 libwebsockets，再链接 OpenSSL
    if(APPLE)
        # macOS 平台使用 OpenSSL 变量
        target_link_libraries(NotificationCenterSDK PUBLIC 
            ${CMAKE_CURRENT_SOURCE_DIR}/third_party/libwebsockets/build/lib/libwebsockets.a
            ${OPENSSL_LIBRARIES}
            Threads::Threads
        )
        target_link_libraries(NotificationCenterSDKShared PUBLIC 
            ${CMAKE_CURRENT_SOURCE_DIR}/third_party/libwebsockets/build/lib/libwebsockets.a
            ${OPENSSL_LIBRARIES}
            Threads::Threads
        )
    else()
        # Linux 平台使用 ssl 和 crypto
        target_link_libraries(NotificationCenterSDK PUBLIC 
            ${CMAKE_CURRENT_SOURCE_DIR}/third_party/libwebsockets/build/lib/libwebsockets.a
            ${OPENSSL_LIBRARIES}
            ssl
            crypto
            Threads::Threads
        )
        target_link_libraries(NotificationCenterSDKShared PUBLIC 
            ${CMAKE_CURRENT_SOURCE_DIR}/third_party/libwebsockets/build/lib/libwebsockets.a
            ${OPENSSL_LIBRARIES}
            ssl
            crypto
            Threads::Threads
        )
    endif()
else()
    # 即使不使用 libwebsockets，也需要链接 pthread
    target_link_libraries(NotificationCenterSDK PUBLIC Threads::Threads)
    target_link_libraries(NotificationCenterSDKShared PUBLIC Threads::Threads)
endif()
