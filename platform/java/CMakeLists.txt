cmake_minimum_required(VERSION 3.10)
project(notificationcenter_jni)

cmake_policy(SET CMP0078 NEW)
cmake_policy(SET CMP0086 NEW)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_JAVA_COMPILE_FLAGS "-source 1.8 -target 1.8")

find_package(JNI REQUIRED)
find_package(SWIG REQUIRED)
find_package(Threads REQUIRED)

include(UseSWIG)

set(CMAKE_SWIG_FLAGS "-c++" "-package" "com.dji.notificationcentersdk.generated")

set(SWIG_MODULE_NAME notificationcenter)
set(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/sdk/src/main/java/com/dji/notificationcentersdk/generated)
set(CMAKE_SWIG_OUT_PREFIX "")

# Interface files
set(SWIG_INTERFACE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/swig/notificationcenter.i
)

# Replace swig_add_library with a more explicit two-step process
# 1. Generate the wrapper sources from the .i file
swig_add_library(${SWIG_MODULE_NAME}
        LANGUAGE java
        SOURCES ${SWIG_INTERFACE_FILES}
)
set_target_properties(${SWIG_MODULE_NAME} PROPERTIES OUTPUT_NAME "notificationcenter")

# After the target is created by swig_add_library, we can get its sources
# and force the C wrapper to be compiled as C++
get_target_property(swig_sources ${SWIG_MODULE_NAME} SOURCES)
foreach(source ${swig_sources})
    if(source MATCHES "_wrap\\.c$")
        set_source_files_properties(${source} PROPERTIES LANGUAGE CXX)
    endif()
endforeach()

# Force the generated wrapper to be compiled as C++
# set_source_files_properties(
#     ${CMAKE_CURRENT_BINARY_DIR}/notificationcenterJAVA_wrap.c
#     PROPERTIES
#     LANGUAGE CXX
# )

# Also set the include directories for SWIG itself
set_property(TARGET ${SWIG_MODULE_NAME} PROPERTY SWIG_INCLUDE_DIRECTORIES
        "${CMAKE_SOURCE_DIR}/src"
        "${CMAKE_SOURCE_DIR}/src/business_manager"
        "${CMAKE_SOURCE_DIR}/src/topic_engine"
        "${CMAKE_SOURCE_DIR}/src/websocket"
)

target_include_directories(${SWIG_MODULE_NAME} PUBLIC
        ${JNI_INCLUDE_DIRS}
        ../../src
        ../../src/business_manager
        ../../src/topic_engine
        ../../src/websocket
        ../../src/third_party/AIGCJson/include
        ../../src/third_party/plog/include
)

target_link_libraries(${SWIG_MODULE_NAME} PRIVATE
        NotificationCenterSDK
        ${JNI_LIBRARIES}
        Threads::Threads
)
