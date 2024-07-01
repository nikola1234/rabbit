include(../../utils/common.cmake)
cmake_minimum_required(VERSION ${CMAKE_VERSION})

set(NAME applicationSwU)

set(${NAME}_SRC
    RabbitMain.cc
)

add_library(${NAME} ${${NAME}_SRC})

target_include_directories( ${NAME} PRIVATE
    inc
    ${REPOROOT}/src/stlSwU/inc
)