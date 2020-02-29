project(tflogger C CXX)
cmake_minimum_required(VERSION 3.12)
include(CMakePackageConfigHelpers)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(TFLOGGER_VERSION_MAJOR 1)
set(TFLOGGER_VERSION_MINOR 0)
set(TFLOGGER_VERSION_PATCH 1)
set(TFLOGGER_VERSION "${TFLOGGER_VERSION_MAJOR}.${TFLOGGER_VERSION_MINOR}.${TFLOGGER_VERSION_PATCH}")

set(TFLOGGER_SOVERSION_MAJOR 1)
set(TFLOGGER_SOVERSION_MINOR 0)
set(TFLOGGER_SOVERSION_PATCH 0)
set(TFLOGGER_SOVERSION "${TFLOGGER_SOVERSION_MAJOR}.${TFLOGGER_SOVERSION_MINOR}.${TFLOGGER_SOVERSION_PATCH}")

if(NOT LIB_INSTALL_DIR)
  set(LIB_INSTALL_DIR lib)
endif()

if(NOT INCLUDE_INSTALL_DIR)
  set(INCLUDE_INSTALL_DIR include)
endif()
find_package(Protobuf REQUIRED)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
set(CMAKE_INCLUDE_CURRENT_DIR ON)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tflogger/proto)
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS
  tflogger/pb/event.proto
  tflogger/pb/summary.proto
  PROTOC_OUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/tflogger/proto)

set(TFLOGGER_SRC
  tflogger/crc32/crc32c.cc
  tflogger/crc32/crc32c_accelerate.cc
  tflogger/record/record_reader.cc
  tflogger/record/record_writer.cc
  tflogger/event_logger.cc
  tflogger/event_reader.cc
  tflogger/utils.cc
  ${PROTO_SRCS})
set(TFLOGGER_HEADERS
  tflogger/record/record_reader.h
  tflogger/record/record_writer.h
  tflogger/event_logger.h
  tflogger/event_reader.h
  tflogger/utils.h)
add_library(tflogger ${TFLOGGER_SRC})
target_link_libraries(tflogger PUBLIC protobuf::libprotobuf)
set_target_properties(tflogger
  PROPERTIES
    SOVERSION ${TFLOGGER_SOVERSION}
    VERSION ${TFLOGGER_VERSION}
)
add_library(tflogger::tflogger ALIAS tflogger)

OPTION(BUILD_EXAMPLES "Build examples")
if(BUILD_EXAMPLES)
  add_subdirectory(examples/cpp)
endif()

OPTION(BUILD_TESTS "Build tests")
if(BUILD_TESTS)
  find_package(Catch2 CONFIG)
  set(THIRD_PARTY_PATH ${CMAKE_SOURCE_DIR}/third_party)
  enable_testing()
  add_subdirectory(test)
endif()

OPTION(INTERFACE_PYTHON "Create Python wrapper")
if(INTERFACE_PYTHON)
  find_package(SWIG REQUIRED)
  add_subdirectory(python)
endif()

INSTALL(FILES ${TFLOGGER_HEADERS} DESTINATION ${INCLUDE_INSTALL_DIR}/tflogger)
INSTALL(
  FILES tflogger/record/record_reader.h tflogger/record/record_writer.h
  DESTINATION ${INCLUDE_INSTALL_DIR}/tflogger/record)
INSTALL(FILES tflogger/crc32/crc32c.h DESTINATION ${INCLUDE_INSTALL_DIR}/tflogger/crc32)
INSTALL(FILES ${PROTO_HDRS} DESTINATION ${INCLUDE_INSTALL_DIR}/tflogger/proto)

INSTALL(
  TARGETS tflogger
  EXPORT TFLoggerTargets
  ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
  LIBRARY DESTINATION ${LIB_INSTALL_DIR}
  INCLUDES DESTINATION ${INCLUDE_INSTALL_DIR}
)

set(CONFIG_PACKAGE_DIR ${LIB_INSTALL_DIR}/cmake/tflogger)
configure_package_config_file(
  ${CMAKE_SOURCE_DIR}/cmake/TFLoggerConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/TFLoggerConfig.cmake
  INSTALL_DESTINATION ${LIB_INSTALL_DIR}/cmake/tflogger
  PATH_VARS INCLUDE_INSTALL_DIR CONFIG_PACKAGE_DIR)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/TFLoggerConfigVersion.cmake"
  VERSION ${TFLOGGER_VERSION}
  COMPATIBILITY SameMajorVersion
)

export(EXPORT TFLoggerTargets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/TFLoggerTargets.cmake"
  NAMESPACE tflogger::
)

install(
  EXPORT TFLoggerTargets
  FILE TFLoggerTargets.cmake
  NAMESPACE tflogger::
  DESTINATION ${CONFIG_PACKAGE_DIR}
)

install(
  FILES
    "${CMAKE_CURRENT_BINARY_DIR}/TFLoggerConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/TFLoggerConfigVersion.cmake"
  DESTINATION ${CONFIG_PACKAGE_DIR}
)

