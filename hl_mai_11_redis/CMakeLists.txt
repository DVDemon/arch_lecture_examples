cmake_minimum_required(VERSION 3.11)
project(hl_mai_11_redis)

SET(EXECUTABLE "redis_test")
SET(EXECUTABLE_PUBSUB "redis_pubsub")
set (STD_CXX "c++17")
set (REDISCPP_FLAGS "-DREDISCPP_HEADER_ONLY=ON")

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -std=${STD_CXX} ${REDISCPP_FLAGS}")
set (CMAKE_CXX_FLAGS_RELEASE "-O3 -g0 -std=${STD_CXX} -Wall -DNDEBUG ${REDISCPP_FLAGS}")
set (CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Boost 1.67.0 REQUIRED COMPONENTS thread system iostreams)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

set (LIBRARIES
    ${LIBRARIES}
    ${Boost_LIBRARIES}
)

add_executable(${EXECUTABLE} main.cpp)
add_executable(${EXECUTABLE_PUBSUB} pubsub_main.cpp)

target_link_libraries(${EXECUTABLE} ${LIBRARIES})
target_link_libraries(${EXECUTABLE_PUBSUB} ${LIBRARIES})