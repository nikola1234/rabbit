include(../utils/common.cmake)
cmake_minimum_required(VERSION ${CMAKE_VERSION})

set(NAME protoInterface)

include(FindProtobuf)
find_package(Protobuf REQUIRED)
if ( Protobuf_FOUND )
    message( STATUS "Protobuf version : ${Protobuf_VERSION}" )
    message( STATUS "Protobuf include path : ${Protobuf_INCLUDE_DIRS}" )
    message( STATUS "Protobuf libraries : ${Protobuf_LIBRARIES}" )
else()
    message( WARNING "Protobuf package not found -> specify search path via PROTOBUF_ROOT variable")
endif()

##
protobuf_generate_cpp(${NAME}_PROTO_SRC ${NAME}_PROTO_HEADER ${PROTO_FILES})

set(${NAME}_SRC
    ${${NAME}_PROTO_HEADER}
    ${${NAME}_PROTO_SRC}
)

add_library(${NAME} ${${NAME}_SRC})
# Dsiable type limits warning for protobuf interfaces
target_compile_options(${NAME} PUBLIC -Wno-type-limits)

target_include_directories( ${NAME} SYSTEM PUBLIC
    ${PROTOBUF_INCLUDE_DIR}
    ${BIN_DIR}/protoInterface
)