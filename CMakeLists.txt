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
    include_directories(/home/hominsu/Library/TensorRT-8.0.3.4/include)
    link_directories(/home/hominsu/Library/TensorRT-8.0.3.4/lib)
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
    # cuda
    include_directories(/usr/local/cuda/include)
    link_directories(/usr/local/cuda/lib64)
    # tensorrt
    include_directories(/home/hominsu/Library/TensorRT-8.0.3.4/include)
    link_directories(/home/hominsu/Library/TensorRT-8.0.3.4/lib)
    # other
    include_directories(/usr/local/include)
    link_directories(/usr/local/lib)
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


# 提示 grpc 版本不对，有两种方法：
#   1. 重新生成将文件覆盖 detect_service 下的文件
#   2. 删除 detect_service 下的 *.pb.cc, *.pb.h，然后取消下面的多行注释，修改对应的 include 路径，使用 cmake --build 命令，
#      会自动生成 *.pb.cc, *.pb.h，导入项目中编译
#[[
# Proto file
get_filename_component(hw_proto "./proto/detect.proto" ABSOLUTE)
get_filename_component(hw_proto_path "${hw_proto}" PATH)

# Generated sources
set(hw_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/detect_service/detect.pb.cc")
set(hw_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/detect_service/detect.pb.h")
set(hw_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/detect_service/detect.grpc.pb.cc")
set(hw_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/detect_service/detect.grpc.pb.h")
add_custom_command(
        OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
        COMMAND ${_PROTOBUF_PROTOC}
        ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${hw_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${hw_proto}"
        DEPENDS "${hw_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# hw_grpc_proto
add_library(hw_grpc_proto
        ${hw_grpc_srcs}
        ${hw_grpc_hdrs}
        ${hw_proto_srcs}
        ${hw_proto_hdrs})
        ]]

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
