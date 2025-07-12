cmake_minimum_required(VERSION 3.0)
project(example_cpp)

set(CMAKE_CXX_STANDARD 17)

# 头文件目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src/websocket)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../src/base)
include(${CMAKE_CURRENT_SOURCE_DIR}/../../src/third_party/ThirdParty.cmake)

# 链接主库目录
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../build_cpp)

add_executable(example_cpp main.cpp)
target_link_libraries(example_cpp NotificationCenterSDK) 

add_executable(thread_pool_test thread_pool_test.cpp)
target_link_libraries(thread_pool_test NotificationCenterSDK) 