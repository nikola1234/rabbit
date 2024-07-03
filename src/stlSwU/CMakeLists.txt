include(../../utils/common.cmake)
cmake_minimum_required(VERSION ${CMAKE_VERSION})

set(NAME stlSwU)

set(${NAME}_SRC
    src/MapTest.cc
    src/ObjConstructor.cc
)

add_library(${NAME} ${${NAME}_SRC})

target_include_directories( ${NAME} PRIVATE
    inc
    ${FWK_DIR}/inc
)