cmake_minimum_required(VERSION 3.0)
project(example_cpp)

set(CMAKE_CXX_STANDARD 17)

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
target_link_libraries(example_cpp NotificationCenterSDK) 

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
target_link_libraries(thread_pool_test NotificationCenterSDK) 