cmake_minimum_required(VERSION 3.2)

project(hl_graphql C CXX)

SET (EXAMPLE_BINARY "hl_graphql")

SET (GRAPHQL_LIBS "graphqljson")
LIST(APPEND GRAPHQL_LIBS "graphqlpeg")
LIST(APPEND GRAPHQL_LIBS "graphqljson")
LIST(APPEND GRAPHQL_LIBS "graphqlresponse")
LIST(APPEND GRAPHQL_LIBS "graphqlservice")

SET (POCO_DATA_LIBS "PocoData")
LIST(APPEND POCO_DATA_LIBS "PocoDataPostgreSQL")

find_package(OpenSSL)
find_package(Threads)
find_package(ZLIB)
find_package(Poco REQUIRED COMPONENTS Foundation Util Net XML JSON Crypto NetSSL)
find_package(Boost COMPONENTS filesystem system program_options regex REQUIRED)

if(NOT ${Poco_FOUND})
    message(FATAL_ERROR "Poco C++ Libraries not found.")
endif()

include_directories(${Poco_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIR})



add_executable(${EXAMPLE_BINARY} main.cpp 
                                 config/config.cpp 
                                 database/database.cpp
                                 GQL/UserObject.cpp
                                 GQL/MutationsObject.cpp
                                 GQL/GQLSchema.cpp
                                 GQL/QueryObject.cpp
                                 database/ServiceMock.cpp)

target_include_directories(${EXAMPLE_BINARY} PRIVATE "${CMAKE_BINARY_DIR}")
target_compile_options(${EXAMPLE_BINARY} PRIVATE -Wall -Wextra -pedantic -Werror -fcoroutines)

target_link_libraries(${EXAMPLE_BINARY} PRIVATE 
                             ${CMAKE_THREAD_LIBS_INIT} 
                             ${Poco_LIBRARIES}
                             ${POCO_DATA_LIBS}
                             ${GRAPHQL_LIBS}
                             ${Boost_LIBRARIES}
                             stdc++fs
                             ZLIB::ZLIB)

set_target_properties(${EXAMPLE_BINARY} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${EXAMPLE_BINARY} PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)