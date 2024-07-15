include(../../../utils/common.cmake)
cmake_minimum_required(VERSION ${CMAKE_VERSION})

set(NAME zhangMessageCSwU)

set(${NAME}_SRC
    src/client.cc
)

add_library(${NAME} ${${NAME}_SRC})

target_include_directories( ${NAME} PRIVATE
    ../common/inc
    inc
    ${FWK_DIR}/inc
    ${BIN_DIR}/protoInterfaces/
)