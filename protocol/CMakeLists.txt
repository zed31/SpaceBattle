cmake_minimum_required(VERSION 3.8)
project(space_battle)

if (CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-value -Wno-unused-parameter -Wno-unused-function -DBOOST_ERROR_CODE_HEADER_ONLY -DASIO_STANDALONE -DBOOST_SYSTEM_NO_DEPRECATED")
elseif (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 -DBOOST_ERROR_CODE_HEADER_ONLY -DASIO_STANDALONE -DBOOST_SYSTEM_NO_DEPRECATED -D_SCL_SECURE_NO_WARNINGS")
endif ()

set(EXTERNAL_LIBS ../external)
set(PROTOCOL protocol)

include_directories(
        ${EXTERNAL_LIBS}/include
        ./include
)

set(
        PROTOCOL_SRC
        ./include/server.hpp
        ./src/server.cpp
        ./include/client.hpp
        ./src/client.cpp
        ./include/serialization/request.hpp
        ./include/serialization/data_utils.hpp
        ./include/serialization/body.hpp
        ./src/serialization/body.cpp
        ./src/serialization/request.cpp
        ./include/serialization/response.hpp
        ./src/serialization/response.cpp
)

add_library(
        libspace_proto
        STATIC
        ${PROTOCOL_SRC}
)

target_include_directories(libspace_proto INTERFACE PUBLIC ${PROTOCOL_API_FOLDER}/include)