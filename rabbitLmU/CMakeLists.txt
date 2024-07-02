project("Rabbit")
include(../utils/common.cmake)
cmake_minimum_required(VERSION ${CMAKE_VERSION})

set(NAME rabbit)

add_executable(${NAME} app.cc)

set(SRC_DIR ${REPOROOT}/src)
set(BIN_DIR ${CMAKE_BINARY_DIR}/bin/) #build/bin

#set(EXECUTABLE_OUTPUT_PATH ${REPOROOT}/rabbitLmU)
#set(LIBARARY_OUTPUT_PATH ${REPOROOT}/rabbitLmU/lib)

#add_subdirectory(${REPOROOT}/base)
#add_subdirectory(${REPOROOT}/net)

#include_directries($(PROJECET_SOURCE_DIR))

# src
add_subdirectory(${SRC_DIR}/applicationSwU ${BIN_DIR}/applicationSwU)
add_subdirectory(${SRC_DIR}/stlSwU ${BIN_DIR}/stlSwU)
add_subdirectory(${SRC_DIR}/algorithmSwU ${BIN_DIR}/algorithmSwU)
add_subdirectory(${FWK_DIR}/ ${BIN_DIR}/)

target_include_directories( ${NAME} PRIVATE
    inc
    ${REPOROOT}/src/stlSwU/inc
    ${REPOROOT}/src/algorithmSwU/inc
    ${REPOROOT}/fwk/inc
)

target_link_libraries( ${NAME} PRIVATE
    -Wl,--whole-archive
    ############## start ################
    fwk
    applicationSwU
    stlSwU
    algorithmSwU
    #### add other SwU

    ############## end   ################
    -Wl,--no-whole-archive
)

