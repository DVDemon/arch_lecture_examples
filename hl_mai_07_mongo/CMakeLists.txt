cmake_minimum_required(VERSION 3.10)

set(EXECUTABLE example_01)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads)
find_package(Poco REQUIRED COMPONENTS Foundation Util Net XML JSON Crypto NetSSL MongoDB)

include_directories(${Poco_INCLUDE_DIRS})
add_executable(${EXECUTABLE} main.cpp)
target_link_libraries(${EXECUTABLE} PRIVATE ${CMAKE_THREAD_LIBS_INIT} ${Poco_LIBRARIES})