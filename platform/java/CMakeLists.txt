cmake_minimum_required(VERSION 3.10)
project(notificationcenter_jni)

set(CMAKE_CXX_STANDARD 17)

find_package(JNI REQUIRED)

# a shared library
add_library(notificationcenter_jni SHARED
        jni/sdk_jni.cpp
        jni/business_manager_jni.cpp
        )

# include directories
target_include_directories(notificationcenter_jni PUBLIC
        ${JNI_INCLUDE_DIRS}
        ../../src
        ../../src/third_party/AIGCJson/include
        ../../src/third_party/plog/include
        )

# link libraries
target_link_libraries(notificationcenter_jni PRIVATE
        NotificationCenterSDK
        ${JNI_LIBRARIES}
        ) 