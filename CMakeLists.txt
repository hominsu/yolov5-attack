cmake_minimum_required(VERSION 3.10)
project(yolov5-attack)

add_definitions(-std=c++17)
option(CUDA_USE_STATIC_CUDA_RUNTIME OFF)
set(CMAKE_CXX_STANDARD 17)

# ============================== select operating system  ==============================
message("")
message("Operation system is ${CMAKE_SYSTEM}")
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    message("Current platform: Linux")
    # cuda
    include_directories(/usr/local/cuda/include)
    link_directories(/usr/local/cuda/lib64)
    # tensorrt
    include_directories(/usr/local/TensorRT-7.0.0.11/include)
    link_directories(/usr/local/TensorRT-7.0.0.11/lib)
    # other
    include_directories(/usr/local/include)
    link_directories(/usr/local/lib)
elseif (CMAKE_SYSTEM_NAME MATCHES "Darwin")
    message("Current platform: Darwin")
    link_directories(/opt/homebrew/lib)
    include_directories(/opt/homebrew/include)
    message(STATUS "Load link directories from /opt/homebrew/lib")
    message(STATUS "Load include directories from /opt/homebrew/include")
    SET(OPENSSL_ROOT_DIR /opt/homebrew/Cellar/openssl@1.1/1.1.1l)
    SET(OPENSSL_INCLUDE_DIR /opt/homebrew/Cellar/openssl@1.1/1.1.1l/include)
elseif (CMAKE_SYSTEM_NAME MATCHES "Windows")
    message("Current platform: Windows")
    add_custom_target(COMMAND ${CMAKE_COMMAND} -E env "PATH=D:/Library/json/build;$ENV{PATH}")
    add_custom_target(COMMAND ${CMAKE_COMMAND} -E env "PATH=D:/Library/opencv/build;$ENV{PATH}")
else ()
    message(FATAL_ERROR "Platform ${CMAKE_SYSTEM_NAME} is not support for this project")
endif ()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Ofast -Wfatal-errors -D_MWAITXINTRIN_H_INCLUDED")
#set(CMAKE_PREFIX_PATH "/home/ynu/.local")

# ============================== find_package ==============================
# Threads
find_package(Threads REQUIRED)
if (Threads_FOUND)
    message("")
endif ()

# nlohmann/json
find_package(nlohmann_json REQUIRED)
if (nlohmann_json_FOUND)
    message("")
endif ()

# OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
if (OpenCV_FOUND)
    message(STATUS "OpenCV_VERSION = ${OpenCV_VERSION}")
    message(STATUS "OpenCV_LIBS = ${OpenCV_LIBS}")
    message(STATUS "OpenCV_INCLUDE_DIRS = ${OpenCV_INCLUDE_DIRS}")
    message("")
endif ()

# Cuda
find_package(CUDA REQUIRED)
if (CUDA_FOUND)
    message(STATUS "CUDA_VERSION_STRING = ${CUDA_VERSION_STRING}")
    message(STATUS "CUDA_TOOLKIT_ROOT_DIR = ${CUDA_TOOLKIT_ROOT_DIR}")
    message(STATUS "CUDA_INCLUDE_DIRS = ${CUDA_INCLUDE_DIRS}")
    message("")
endif ()


file(GLOB ProjectSRC
        "attack/*.cc"
        "config/*.cc"
        "model/*.cc"
        "utils/*.cc"
        "*.cc")

if (CMAKE_BUILD_TYPE STREQUAL Debug)
    ADD_DEFINITIONS(-DDebug)
    message(STATUS "CMake Build Type: Debug")
    message("")
elseif (CMAKE_BUILD_TYPE STREQUAL Release)
    ADD_DEFINITIONS(-DRelease)
    message(STATUS "CMake Build Type: Release")
    message("")
endif ()


# 创建静态链接库，也可以生成动态的（SHARED）
cuda_add_library(YoloV5 STATIC model/yololayer.cu)
target_link_libraries(YoloV5 nvinfer cudart)

add_executable(${PROJECT_NAME} ${ProjectSRC})
# ${hw_grpc_proto}
target_link_libraries(${PROJECT_NAME} nvinfer cudart YoloV5 ${OpenCV_LIBS} Threads::Threads)
