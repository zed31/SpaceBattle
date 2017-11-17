cmake_minimum_required(VERSION 3.8)
project(space_battle)

if (CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-value -Wno-unused-parameter -Wno-unused-function -DBOOST_ERROR_CODE_HEADER_ONLY -DASIO_STANDALONE -DBOOST_SYSTEM_NO_DEPRECATED")
elseif (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 -DBOOST_ERROR_CODE_HEADER_ONLY -DASIO_STANDALONE -DBOOST_SYSTEM_NO_DEPRECATED -D_SCL_SECURE_NO_WARNINGS")
endif ()

find_package(Threads REQUIRED)
set(CMAKE_THREAD_PREFER_PTHREAD ON)
set(SERVER_INCLUDE ${SERVER_DIR})
set(ASIO_INCLUDE ../../external)
set(PROTOCOL_INCLUDES ../../protocol)
set(TEST_SRC ./client)

include_directories(
        ${ASIO_INCLUDE}/include
        ${PROTOCOL_INCLUDES}/include
)

set(
        CLIENT_STUB_SRC
        client/client.cpp)
add_executable(client_stub ${CLIENT_STUB_SRC})
target_link_libraries(
        client_stub
        libspace_proto
        Threads::Threads
)