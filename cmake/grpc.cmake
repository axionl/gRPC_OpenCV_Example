# Find gRPC Package
find_package(gRPC CONFIG REQUIRED)

# Find Generator Executable Plugin
find_program(GRPC_CC_PLUGIN_EXECUTABLE grpc_cpp_plugin)

# Find PkgConfig
find_package(PkgConfig REQUIRED)

# Check Modules
pkg_check_modules(GRPC REQUIRED grpc++ grpc)

# Set Source Files
set(API_GRPC_SRCS ${CMAKE_CURRENT_BINARY_DIR}/${API_PROTO_NAME}.grpc.pb.cc)
set(API_GRPC_HDRS ${CMAKE_CURRENT_BINARY_DIR}/${API_PROTO_NAME}.grpc.pb.h)

# Set Libraries
set(GRPC_GRPCPP_LIB gRPC::grpc++)
set(GRPC_REFLECTION_LIB gRPC::grpc++_reflection)

# Generate gRPC Sources
add_custom_command(
        OUTPUT "${API_GRPC_SRCS}" "${API_GRPC_HDRS}" "${API_PROTO_SRCS}" "${API_PROTO_HDRS}"
        COMMAND ${PROTOBUF_PROTOC_EXECUTABLE}
        ARGS --grpc_out=${CMAKE_CURRENT_BINARY_DIR}
            --cpp_out=${CMAKE_CURRENT_BINARY_DIR}
            -I="${API_PROTO_PATH}"
            --plugin=protoc-gen-grpc="${GRPC_CC_PLUGIN_EXECUTABLE}"
            "${API_PROTO}"
        DEPENDS "${API_PROTO}"
)