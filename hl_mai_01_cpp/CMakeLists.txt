cmake_minimum_required(VERSION 3.10)

SET (EXAMPLE_BINARY "hl_mai_01")

find_package(Threads)

add_executable(${EXAMPLE_BINARY} main.cpp new_lib.cpp)
add_library(library config.cpp)

target_include_directories(${EXAMPLE_BINARY} PRIVATE "${CMAKE_BINARY_DIR}")
target_include_directories(library PRIVATE "${CMAKE_BINARY_DIR}")

target_compile_options(${EXAMPLE_BINARY} PRIVATE -Wall -Wextra -pedantic -Werror )
target_compile_options(library PRIVATE -Wall -Wextra -pedantic -Werror )


target_link_libraries(${EXAMPLE_BINARY} PRIVATE library ${CMAKE_THREAD_LIBS_INIT})

set_target_properties(${EXAMPLE_BINARY} library PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${EXAMPLE_BINARY} library PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)


