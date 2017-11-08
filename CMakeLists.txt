cmake_minimum_required(VERSION 3.8)
project(space_battle)

set(GOOGLETEST_ROOT external/googletest/googletest CACHE STRING "Google Test source root")
set(EXTERNAL_LIBS external)
set(PROTOCOL protocol)
if (CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-value -Wno-unused-parameter -Wno-unused-function -DBOOST_ERROR_CODE_HEADER_ONLY -DASIO_STANDALONE -DBOOST_SYSTEM_NO_DEPRECATED")
elseif (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 -DBOOST_ERROR_CODE_HEADER_ONLY -DASIO_STANDALONE -DBOOST_SYSTEM_NO_DEPRECATED -D_SCL_SECURE_NO_WARNINGS")
endif ()

include_directories(
        ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}
        ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}/include
        ${EXTERNAL_LIBS}/include
        ${PROTOCOL}/include
)

set(GOOGLETEST_SOURCES
        ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}/src/gtest-all.cc
        ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}/src/gtest_main.cc
        )

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

set(
        PROTOCOL_SRC
        protocol/include/NetIOBusinessData.hpp
        protocol/include/NetEngine.hpp
        protocol/include/Net.hpp
        protocol/test/NetTest.cpp
        protocol/src/NetIOBusinessData.cpp)

add_executable(
        protocole_test
        protocol/test/serializationTest.cpp
        ${PROTOCOL_SRC}
        protocol/include/NetIO.hpp
        protocol/src/Net.cpp
        server/test/test_main.cpp)

target_include_directories(protocole_test INTERFACE PUBLIC ${PROTOCOL_API_FOLDER}/include)

add_dependencies(protocole_test googletest)
target_link_libraries(
        protocole_test
        googletest
        Threads::Threads
)

if(MINGW)
    target_link_libraries(protocole_test user32 wsock32 ws2_32)
endif()

#include(CTest)
#enable_testing()
#add_test(unit ${PROJECT_BINARY_DIR}/unit_tests)