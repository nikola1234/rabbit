project("Rabbit")
include(../utils/common.cmake)
cmake_minimum_required(VERSION ${CMAKE_VERSION})

set(NAME rabbit)

add_executable(${NAME} app.cc)

set(SRC_DIR ${REPOROOT}/src)
#set(FWK_DIR ${REPOROOT}/fwk)
set(BIN_DIR ${CMAKE_BINARY_DIR}/bin/) #build/bin

#set(EXECUTABLE_OUTPUT_PATH ${REPOROOT}/rabbitLmU)
#set(LIBARARY_OUTPUT_PATH ${REPOROOT}/rabbitLmU/lib)

#add_subdirectory(${REPOROOT}/base)
#add_subdirectory(${REPOROOT}/net)

#include_directries($(PROJECET_SOURCE_DIR))

# src
add_subdirectory(${SRC_DIR}/applicationSwU ${BIN_DIR}/applicationSwU)
add_subdirectory(${SRC_DIR}/stlSwU ${BIN_DIR}/stlSwU)

target_include_directories( ${NAME} PRIVATE
    inc
    ${REPOROOT}/src/stlSwU/inc
)

target_link_libraries( ${NAME} PRIVATE
    -Wl,--whole-archive
    ############## start ################
    applicationSwU
    stlSwU
    
    #### add other SwU

    ############## end   ################
    -Wl,--no-whole-archive
)

