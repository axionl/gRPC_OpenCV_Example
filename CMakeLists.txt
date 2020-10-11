# Set Cmake Minimum Version
cmake_minimum_required(VERSION 3.10.1)

# Set Project Name
project(gRPC_OpenCV_Example C CXX)

# Set Cpp Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Threads
find_package(Threads REQUIRED)

# 3rdparty Sources
include(cmake/protobuf.cmake)
include(cmake/grpc.cmake)
include(cmake/opencv.cmake)

# Targets greeter_[async_](client|server)
foreach(_target
        greeter_client
        greeter_async_client
        greeter_server
        greeter_async_server)
    add_executable(${_target}
            "${_target}.cc"
            ${API_PROTO_SRCS}
            ${API_GRPC_SRCS}
            mat_converter.cpp
            mat_converter.h)

    target_link_libraries(${_target}
            ${GRPC_GRPCPP_LIB}
            ${PROTOBUF_LIBPROTOBUF_LIB}
            ${GRPC_REFLECTION_LIB}
            ${OpenCV_LIBS})
endforeach()

# Include Sources
include_directories(${CMAKE_CURRENT_BINARY_DIR})