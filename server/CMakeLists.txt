cmake_minimum_required(VERSION 3.8)
project(space_battle)

if (CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-value -Wno-unused-parameter -Wno-unused-function -DBOOST_ERROR_CODE_HEADER_ONLY -DASIO_STANDALONE -DBOOST_SYSTEM_NO_DEPRECATED")
elseif (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 -DBOOST_ERROR_CODE_HEADER_ONLY -DASIO_STANDALONE -DBOOST_SYSTEM_NO_DEPRECATED -D_SCL_SECURE_NO_WARNINGS")
endif ()

find_package(Threads REQUIRED)
set(CMAKE_THREAD_PREFER_PTHREAD ON)
set(SERVER_DIR ./)
set(SERVER_PATH_SRC ${SERVER_DIR}/src)
set(SERVER_INCLUDE ${SERVER_DIR})
set(ASIO_INCLUDE ../external)
set(PROTOCOL_INCLUDES ../protocol/)

include_directories(
        ${ASIO_INCLUDE}/include
        ${PROTOCOL_INCLUDES}/include
        ${SERVER_INCLUDE}/include
)

set(
        SERVER_SRC
        ${SERVER_PATH_SRC}/main.cpp
        include/server_engine.hpp src/server_engine.cpp include/input_connection_pool.hpp src/input_connection_pool.cpp include/protocol.hpp src/protocol.cpp)

add_executable(server ${SERVER_SRC})
target_link_libraries(
        server
        space_proto
        Threads::Threads
)