project("Rabbit")
include(../utils/common.cmake)
cmake_minimum_required(VERSION ${CMAKE_VERSION})

set(NAME rabbit)
set(ZNAME_SERVER zserver)
set(ZNAME_CLIENT zclient)

add_executable(${NAME} app.cc)

add_executable(${ZNAME_SERVER} app.cc)
add_executable(${ZNAME_CLIENT} app.cc)

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
add_subdirectory(${PROTO_DIR}/ ${BIN_DIR}/protoInterfaces)
add_subdirectory(${SRC_DIR}/zhangMessageSwU/client ${BIN_DIR}/zhangMessageSwU/client)
add_subdirectory(${SRC_DIR}/zhangMessageSwU/server ${BIN_DIR}/zhangMessageSwU/server)

####### Rabbit app#######
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
    protoInterface
    applicationSwU
    stlSwU
    algorithmSwU
    protobuf
    #### add other SwU

    ############## end   ################
    -Wl,--no-whole-archive
)

####### Zmessage Server app#######
target_include_directories( ${ZNAME_SERVER} PRIVATE
    inc
    ${REPOROOT}/fwk/inc
)

target_link_libraries( ${ZNAME_SERVER} PRIVATE
    -Wl,--whole-archive
    ############## start ################
    fwk
    protoInterface
    zhangMessageSSwU
    protobuf
    ############## end   ################
    -Wl,--no-whole-archive
)

####### Zmessage Cleint app#######
target_include_directories( ${ZNAME_CLIENT} PRIVATE
    inc
    ${REPOROOT}/fwk/inc
)

target_link_libraries( ${ZNAME_CLIENT} PRIVATE
    -Wl,--whole-archive
    ############## start ################
    fwk
    protoInterface
    zhangMessageCSwU
    protobuf
    ############## end   ################
    -Wl,--no-whole-archive
)



