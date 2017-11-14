cmake_minimum_required(VERSION 3.8)
project(space_battle)

if (CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-value -Wno-unused-parameter -Wno-unused-function -DBOOST_ERROR_CODE_HEADER_ONLY -DASIO_STANDALONE -DBOOST_SYSTEM_NO_DEPRECATED")
elseif (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 -DBOOST_ERROR_CODE_HEADER_ONLY -DASIO_STANDALONE -DBOOST_SYSTEM_NO_DEPRECATED -D_SCL_SECURE_NO_WARNINGS")
endif ()

set(GOOGLETEST_ROOT external/googletest/googletest CACHE STRING "Google Test source root")
set(EXTERNAL_LIBS external)
set(PROTOCOL protocol)
set(PROTOCOL_API_FOLDER protocol)

include_directories(
        ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}
        ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}/include
        ${EXTERNAL_LIBS}/include
        ${PROTOCOL}/include
        ${PROTOCOL_API_FOLDER}/include
)

set(
        GOOGLETEST_SOURCES
        ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}/src/gtest-all.cc
        ${PROJECT_SOURCE_DIR}/${GOOGLETEST_ROOT}/src/gtest_main.cc
        server/src/main.cpp)

foreach(_source ${GOOGLETEST_SOURCES})
    set_source_files_properties(${_source} PROPERTIES GENERATED 1)
endforeach()

add_library(googletest ${GOOGLETEST_SOURCES})

find_package(Threads REQUIRED)
set(CMAKE_THREAD_PREFER_PTHREAD ON)

#################################################
#PROTOCOLE
#################################################

set(
        PROTOCOL_SRC
        protocol/include/server.hpp
        protocol/src/server.cpp
        protocol/include/client.hpp
        protocol/src/client.cpp
        protocol/include/serialization/request.hpp
        protocol/include/serialization/data_utils.hpp
        protocol/include/serialization/body.hpp
        protocol/src/serialization/body.cpp
        protocol/src/serialization/request.cpp
        protocol/include/serialization/response.hpp
        protocol/src/serialization/response.cpp
)

add_library(
        space_proto
        ${PROTOCOL_SRC}
)

target_include_directories(space_proto INTERFACE PUBLIC ${PROTOCOL_API_FOLDER}/include)


#################################################
#SERVER SAMPLE
#################################################

add_executable(
       server_sample
        protocol/test/stubs/server.cpp
)

target_include_directories(server_sample INTERFACE PUBLIC ${PROTOCOL_API_FOLDER}/include)
target_link_libraries(
        server_sample
        space_proto
        Threads::Threads
)

#################################################
#CLIENT SAMPLE
#################################################

add_executable(
        client_sample
        protocol/test/stubs/client.cpp
)

target_include_directories(client_sample INTERFACE PUBLIC ${PROTOCOL_API_FOLDER}/include)

target_link_libraries(
        client_sample
        space_proto
        Threads::Threads
)

#################################################
#UNIT TESTS
#################################################

add_executable(
        protocole_unit_test
        protocol/test/unit_test/data_serialization.cpp
        protocol/test/unit_test/test_protocole.cpp
)

target_include_directories(protocole_unit_test INTERFACE PUBLIC ${PROTOCOL_API_FOLDER}/include)

target_link_libraries(
        protocole_unit_test
        space_proto
        googletest
        Threads::Threads
)

##################################################
#SERVER
##################################################
add_subdirectory(server)

if(MINGW)
    target_link_libraries(protocole_test user32 wsock32 ws2_32)
endif()