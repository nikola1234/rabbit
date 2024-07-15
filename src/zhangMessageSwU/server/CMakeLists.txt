include(../../../utils/common.cmake)
cmake_minimum_required(VERSION ${CMAKE_VERSION})

set(NAME zhangMessageSSwU)

set(${NAME}_SRC
    src/server.cc
)

add_library(${NAME} ${${NAME}_SRC})

target_include_directories( ${NAME} PRIVATE
    inc
    ../common/inc
    ${FWK_DIR}/inc
    ${BIN_DIR}/protoInterfaces/
)