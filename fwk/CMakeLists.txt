include(../utils/common.cmake)
cmake_minimum_required(VERSION ${CMAKE_VERSION})

set(NAME fwk)

set(${NAME}_SRC
    src/logging.cc
)

add_library(${NAME} ${${NAME}_SRC})

target_include_directories( ${NAME} PRIVATE
    inc
)