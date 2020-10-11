# Find Protobuf
find_package(Protobuf REQUIRED)

# Find Generator Executable
find_program(PROTOBUF_PROTOC_EXECUTABLE protoc)

# Set Proto Name
set(API_PROTO_NAME "IPCProto")

# Set Libraries
set(PROTOBUF_LIBPROTOBUF_LIB protobuf::libprotobuf)

# Set Proto Path
get_filename_component(API_PROTO_PATH "protofile" ABSOLUTE)

# File Proto File
get_filename_component(API_PROTO "${API_PROTO_PATH}/${API_PROTO_NAME}.proto" ABSOLUTE)

# Generated Sources
protobuf_generate_cpp(API_PROTO_SRCS API_PROTO_HDRS "${API_PROTO}")
