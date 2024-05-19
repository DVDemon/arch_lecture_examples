cmake_minimum_required(VERSION 3.2)

find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)

if(OPENSSL_FOUND)
    set(HTTPLIB_IS_USING_OPENSSL TRUE)
endif()

set(SERVER_NAME "service")

add_executable(${SERVER_NAME} service.cpp)

target_link_libraries(${SERVER_NAME} ${CMAKE_THREAD_LIBS_INIT}
                                     OpenSSL::SSL
                                     OpenSSL::Crypto)