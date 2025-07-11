include_guard(GLOBAL)
# third_party.cmake
# 用于统一管理 third_party 依赖和平台相关路径

# 平台相关库路径
if(CMAKE_SYSTEM_NAME STREQUAL "Android")
    set(LIB_RELATE_FOLDER android/${ANDROID_ABI})
elseif(CMAKE_SYSTEM_NAME STREQUAL "iOS")
    set(LIB_RELATE_FOLDER ios/os64)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(LIB_RELATE_FOLDER macosx/${CMAKE_HOST_SYSTEM_PROCESSOR})
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(LIB_RELATE_FOLDER windows)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    if(CMAKE_HOST_SYSTEM_PROCESSOR STREQUAL aarch64)
        set(LIB_RELATE_FOLDER linux/aarch64)
    else()
        set(LIB_RELATE_FOLDER linux/amd64)
    endif()
endif()

message("LIB_RELATE_FOLDER is ${LIB_RELATE_FOLDER}")

# 平台标志
if(CMAKE_SYSTEM_NAME STREQUAL "Android")
    set(ANDROID true)
    set(LINUX true)
elif(CMAKE_SYSTEM_NAME STREQUAL "iOS")
    set(IOS true)
elif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(MACOSX true)
elif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(WINDOWS true)
elif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(LINUX true)
endif()

set(THIRD_PARTIES_SRC_DIR ${CMAKE_CURRENT_LIST_DIR})

add_library(third_party_dummy_lib INTERFACE)

# ------------------------- AIGCJson ------------------------- #
include_directories(${THIRD_PARTIES_SRC_DIR}/AIGCJson/include)
include_directories(${THIRD_PARTIES_SRC_DIR}/AIGCJson/include/rapidjson)
include_directories(${THIRD_PARTIES_SRC_DIR}/AIGCJson/include/rapidjson/error)
include_directories(${THIRD_PARTIES_SRC_DIR}/AIGCJson/include/rapidjson/internal) 

# ------------------------- plog ------------------------- #
include_directories(${THIRD_PARTIES_SRC_DIR}/plog/include) 

