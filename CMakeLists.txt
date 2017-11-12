cmake_minimum_required(VERSION 3.8)
project(space_battle)

set(GOOGLETEST_ROOT external/googletest/googletest CACHE STRING "Google Test source root")
set(EXTERNAL_LIBS external)
set(PROTOCOL protocol)
if (CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-value -Wno-unused-parameter -Wno-unused-function -DBOOST_ERROR_CODE_HEADER_ONLY -DASIO_STANDALONE -DBOOST_SYSTEM_NO_DEPRECATED")
elseif (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 -DBOOST_ERROR_CODE_HEADER_ONLY -DASIO_STANDALONE -DBOOST_SYSTEM_NO_DEPRECATED -D_SCL_SECURE_NO_WARNINGS")
endif ()

include_directories(
        ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}
        ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}/include
        ${EXTERNAL_LIBS}/include
        ${PROTOCOL}/include
        ${PROTOCOL_API_FOLDER}/include
)

set(GOOGLETEST_SOURCES
        ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}/src/gtest-all.cc
        ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}/src/gtest_main.cc
        protocol/include/Client.h)

foreach(_source ${GOOGLETEST_SOURCES})
    set_source_files_properties(${_source} PROPERTIES GENERATED 1)
endforeach()

add_library(googletest ${GOOGLETEST_SOURCES})

set(TEST_FOLDER ./server/test)
set(TEST_FOLDER_UTILS_CLIENT ./server/test/client)
set(PROTOCOL_API_FOLDER protocol)

add_executable(
        space_battle_server_test
        ${TEST_FOLDER}/simple_test.cpp
        ${TEST_FOLDER_UTILS_CLIENT}/SimpleClient.cpp
)
target_include_directories(space_battle_server_test INTERFACE PUBLIC ${TEST_FOLDER}/include)
target_include_directories(space_battle_server_test INTERFACE PUBLIC ${PROTOCOL_API_FOLDER}/include)

set(CMAKE_THREAD_PREFER_PTHREAD ON)
find_package(Threads REQUIRED)
add_dependencies(space_battle_server_test googletest)
target_link_libraries(
        space_battle_server_test
        googletest
        Threads::Threads
)

#################################################
#PROTOCOLE
#################################################

set(
        PROTOCOL_SRC
        protocol/include/Server.hpp
        protocol/src/Server.cpp
        protocol/include/Client.hpp protocol/src/Client.cpp)

add_executable(
        protocole_test
        ${PROTOCOL_SRC}
        protocol/test/server.cpp
)

target_include_directories(protocole_test INTERFACE PUBLIC ${PROTOCOL_API_FOLDER}/include)

add_dependencies(protocole_test googletest)
target_link_libraries(
        protocole_test
        googletest
        Threads::Threads
)

#################################################
#SERVER
#################################################

set(
        PROTOCOL_SERVER_SRC
        protocol/include/Server.hpp
        protocol/src/Server.cpp
)

add_executable(
        server
        ${PROTOCOL_SERVER_SRC}
        protocol/test/server.cpp
)

target_include_directories(server INTERFACE PUBLIC ${PROTOCOL_API_FOLDER}/include)

add_dependencies(server googletest)
target_link_libraries(
        server
        googletest
        Threads::Threads
)

#################################################
#CLIENT
#################################################

set(
        PROTOCOL_CLIENT_SRC
        protocol/include/Client.hpp
        protocol/src/Client.cpp
)

add_executable(
        client
        ${PROTOCOL_CLIENT_SRC}
        protocol/test/client.cpp
        protocol/test/client.cpp)

target_include_directories(client INTERFACE PUBLIC ${PROTOCOL_API_FOLDER}/include)

add_dependencies(client googletest)
target_link_libraries(
        client
        googletest
        Threads::Threads
)


if(MINGW)
    target_link_libraries(protocole_test user32 wsock32 ws2_32)
endif()

#include(CTest)
#enable_testing()
#add_test(unit ${PROJECT_BINARY_DIR}/unit_tests)