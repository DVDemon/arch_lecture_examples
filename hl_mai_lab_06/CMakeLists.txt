cmake_minimum_required(VERSION 3.2)

SET (EXAMPLE_BINARY "hl_mai_lab_06")
SET (EXAMPLE_GATEWAY "hl_mai_lab_06_gateway")
SET (EXAMPLE_WRITER "hl_mai_lab_06_writer")


find_package(OpenSSL)
find_package(Threads)
find_package(Poco REQUIRED COMPONENTS Foundation Util Net XML JSON Crypto NetSSL Data JWT MongoDB)

if(NOT ${Poco_FOUND})
    message(FATAL_ERROR "Poco C++ Libraries not found.")
endif()

include_directories(${Poco_INCLUDE_DIRS})

set (STD_CXX "c++17")
set (REDISCPP_FLAGS "-DREDISCPP_HEADER_ONLY=ON")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -std=${STD_CXX} ${REDISCPP_FLAGS}")
set (CMAKE_CXX_FLAGS_RELEASE "-O3 -g0 -std=${STD_CXX} -Wall -DNDEBUG ${REDISCPP_FLAGS}")
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${EXAMPLE_WRITER}  pizza_writer/main.cpp
                                  config/config.cpp 
                                  database/database.cpp
                                  database/pizza_order.cpp
                                  database/cache.cpp
                                  database/queue.cpp
                                  rabbit/SimplePocoHandler.cpp)

add_executable(${EXAMPLE_BINARY} main.cpp 
                                 config/config.cpp 
                                 database/database.cpp
                                 database/user.cpp
                                 database/pizza_order.cpp
                                 database/cache.cpp
                                 database/queue.cpp
                                 rabbit/SimplePocoHandler.cpp)


add_executable(${EXAMPLE_GATEWAY} api_gateway/main.cpp
                                 config/config.cpp 
                                 database/cache.cpp)

target_include_directories(${EXAMPLE_BINARY} PRIVATE "${CMAKE_BINARY_DIR}")
target_include_directories(${EXAMPLE_GATEWAY} PRIVATE "${CMAKE_BINARY_DIR}")
target_include_directories(${EXAMPLE_WRITER} PRIVATE "${CMAKE_BINARY_DIR}")

set_target_properties(${EXAMPLE_BINARY} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${EXAMPLE_BINARY} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

target_link_libraries(${EXAMPLE_BINARY} PRIVATE 
                             ${CMAKE_THREAD_LIBS_INIT} 
                             ${Poco_LIBRARIES}
                             "PocoDataPostgreSQL"
                             amqpcpp)

target_link_libraries(${EXAMPLE_GATEWAY} PRIVATE 
                             ${CMAKE_THREAD_LIBS_INIT} 
                             ${Poco_LIBRARIES})

target_link_libraries(${EXAMPLE_WRITER} PRIVATE 
                             ${CMAKE_THREAD_LIBS_INIT} 
                             ${Poco_LIBRARIES}
                             "PocoDataPostgreSQL"
                             amqpcpp)

