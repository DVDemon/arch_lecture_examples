cmake_minimum_required(VERSION 3.2)

find_package(Threads)
find_package(Poco REQUIRED COMPONENTS Foundation Util Net XML JSON Crypto NetSSL)

if(NOT ${Poco_FOUND})
    message(FATAL_ERROR "Poco C++ Libraries not found.")
endif()

include_directories(${Poco_INCLUDE_DIRS})

add_executable(poco_thread poco_thread.cpp)
add_executable(poco_socket poco_socket.cpp)
add_executable(poco_socket_https poco_socket_https.cpp)
add_executable(poco_server poco_server.cpp)

target_link_libraries(poco_thread PRIVATE ${CMAKE_THREAD_LIBS_INIT} ${Poco_LIBRARIES})
target_link_libraries(poco_socket PRIVATE ${CMAKE_THREAD_LIBS_INIT} ${Poco_LIBRARIES})
target_link_libraries(poco_socket_https PRIVATE ${CMAKE_THREAD_LIBS_INIT} ${Poco_LIBRARIES})
target_link_libraries(poco_server PRIVATE ${CMAKE_THREAD_LIBS_INIT} ${Poco_LIBRARIES})


set_target_properties(poco_thread 
                      poco_socket
                      poco_socket_https 
                      poco_server
                     PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)



