include(../../utils/common.cmake)
cmake_minimum_required(VERSION ${CMAKE_VERSION})

set(NAME mMessageSwU)

set(${NAME}_SRC
    common/   # as libmm
    server/   # as a server application
    client/   # as a client application
)

add_library(${NAME} ${${NAME}_SRC})

target_include_directories( ${NAME} PRIVATE
    inc
    ${FWK_DIR}/inc
)