cmake_minimum_required(VERSION 3.5.1)

project(Author C CXX)

include(./common.cmake)

# Proto file
get_filename_component(au_proto "./author.proto" ABSOLUTE)
get_filename_component(au_proto_path "${au_proto}" PATH)

# Generated sources
set(au_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/author.pb.cc")
set(au_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/author.pb.h")
set(au_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/author.grpc.pb.cc")
set(au_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/author.grpc.pb.h")

message(STATUS "Execute custom command ")
add_custom_command(
      OUTPUT "${au_proto_srcs}" "${au_proto_hdrs}" "${au_grpc_srcs}" "${au_grpc_hdrs}"
      COMMAND protoc
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${au_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${au_proto}"
      DEPENDS "${au_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# hw_grpc_proto
add_library(au_grpc_proto
  ${au_grpc_srcs}
  ${au_grpc_hdrs}
  ${au_proto_srcs}
  ${au_proto_hdrs})
target_link_libraries(au_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})

# Targets greeter_[async_](client|server)
foreach(_target
  author_client author_server)
  add_executable(${_target} "${_target}.cc")
  target_link_libraries(${_target}
    au_grpc_proto
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})
endforeach()
