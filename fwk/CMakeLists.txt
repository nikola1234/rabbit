include(../utils/common.cmake)
cmake_minimum_required(VERSION ${CMAKE_VERSION})

set(NAME fwk)

set(${NAME}_SRC
    src/Logging.cc
    src/LogStream.cc
)

add_library(${NAME} ${${NAME}_SRC})

target_include_directories( ${NAME} PRIVATE
    inc
)