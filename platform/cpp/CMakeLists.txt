cmake_minimum_required(VERSION 3.0)
project(example_cpp)

set(CMAKE_CXX_STANDARD 17)

# 查找 pthread 库
find_package(Threads REQUIRED)

# 包含第三方库配置
include(${CMAKE_CURRENT_SOURCE_DIR}/../../src/third_party/ThirdParty.cmake)

# 链接主库目录
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../build_cpp)

add_executable(example_cpp main.cpp)
target_include_directories(example_cpp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/websocket
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/base
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/third_party/plog/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/third_party/AIGCJson/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/third_party/AIGCJson/include/rapidjson
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/third_party/AIGCJson/include/rapidjson/error
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/third_party/AIGCJson/include/rapidjson/internal
)
target_link_libraries(example_cpp NotificationCenterSDK Threads::Threads) 

add_executable(thread_pool_test thread_pool_test.cpp)
target_include_directories(thread_pool_test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/websocket
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/base
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/third_party/plog/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/third_party/AIGCJson/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/third_party/AIGCJson/include/rapidjson
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/third_party/AIGCJson/include/rapidjson/error
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/third_party/AIGCJson/include/rapidjson/internal
)
target_link_libraries(thread_pool_test NotificationCenterSDK Threads::Threads) 

add_executable(timer_test timer_test.cpp)
target_include_directories(timer_test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/websocket
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/base
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/third_party/plog/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/third_party/AIGCJson/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/third_party/AIGCJson/include/rapidjson
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/third_party/AIGCJson/include/rapidjson/error
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/third_party/AIGCJson/include/rapidjson/internal
)
target_link_libraries(timer_test NotificationCenterSDK Threads::Threads)


add_executable(libwebsockets_test libwebsockets_test.cpp)
add_definitions(-DENABLE_LIBWEBSOCKETS)
target_include_directories(libwebsockets_test PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../../src
        ${CMAKE_CURRENT_SOURCE_DIR}/../../src/websocket
        ${CMAKE_CURRENT_SOURCE_DIR}/../../src/base
        ${CMAKE_CURRENT_SOURCE_DIR}/../../src/third_party/plog/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../../src/third_party/AIGCJson/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../../src/third_party/AIGCJson/include/rapidjson
        ${CMAKE_CURRENT_SOURCE_DIR}/../../src/third_party/AIGCJson/include/rapidjson/error
        ${CMAKE_CURRENT_SOURCE_DIR}/../../src/third_party/AIGCJson/include/rapidjson/internal
)
# 链接 openssl 和 libwebsockets
find_package(OpenSSL REQUIRED)
target_include_directories(libwebsockets_test PRIVATE ${OPENSSL_INCLUDE_DIR})
file(REAL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../src/third_party/libwebsockets/build/lib/libwebsockets.a" LIBWEBSOCKETS_ABS_PATH)
target_link_libraries(libwebsockets_test
    NotificationCenterSDK
    ${OPENSSL_LIBRARIES}
    ${LIBWEBSOCKETS_ABS_PATH}
    Threads::Threads
)


add_executable(notification_center_sdk_test notification_center_sdk_test.cpp)
add_definitions(-DENABLE_LIBWEBSOCKETS)
target_include_directories(notification_center_sdk_test PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../../src
        ${CMAKE_CURRENT_SOURCE_DIR}/../../src/websocket
        ${CMAKE_CURRENT_SOURCE_DIR}/../../src/base
        ${CMAKE_CURRENT_SOURCE_DIR}/../../src/third_party/plog/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../../src/third_party/AIGCJson/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../../src/third_party/AIGCJson/include/rapidjson
        ${CMAKE_CURRENT_SOURCE_DIR}/../../src/third_party/AIGCJson/include/rapidjson/error
        ${CMAKE_CURRENT_SOURCE_DIR}/../../src/third_party/AIGCJson/include/rapidjson/internal
)
# 链接 openssl 和 libwebsockets
find_package(OpenSSL REQUIRED)
target_include_directories(notification_center_sdk_test PRIVATE ${OPENSSL_INCLUDE_DIR})
file(REAL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../src/third_party/libwebsockets/build/lib/libwebsockets.a" LIBWEBSOCKETS_ABS_PATH)
target_link_libraries(notification_center_sdk_test
        NotificationCenterSDK
        ${OPENSSL_LIBRARIES}
        ${LIBWEBSOCKETS_ABS_PATH}
        Threads::Threads
)