find_package (Python COMPONENTS Interpreter Development)

INCLUDE(UseSWIG)
IF(${Python_VERSION_MAJOR} VERSION_EQUAL 3)
  SET(TARGET_SWIGFLAGS "-py3")
ELSE()
  MESSAGE(FATAL_ERROR "Python interface of tflogger requires Python version 3!")
ENDIF()

set(PY_PACKAGE_DIR ${CMAKE_CURRENT_BINARY_DIR}/tflogger)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/package_init.py ${PY_PACKAGE_DIR}/__init__.py COPYONLY)
set(PROTO_PY_OUT ${PY_PACKAGE_DIR}/pb)
file(MAKE_DIRECTORY ${PROTO_PY_OUT})
protobuf_generate_python(PROTO_PY
  ${CMAKE_SOURCE_DIR}/tflogger/pb/event.proto
  ${CMAKE_SOURCE_DIR}/tflogger/pb/summary.proto
  PROTOC_OUT_DIR ${PROTO_PY_OUT})
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/pb_init.py ${PROTO_PY_OUT}/__init__.py COPYONLY)

include_directories(${Python_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR})
SET_SOURCE_FILES_PROPERTIES(tflogger.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(tflogger.i PROPERTIES SWIG_FLAGS ${TARGET_SWIGFLAGS})
SET_SOURCE_FILES_PROPERTIES(tflogger.i DEPENDS ${PROTO_PY})
SWIG_ADD_LIBRARY(tflogger_python LANGUAGE python SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/tflogger.i ${PROTO_PY})
swig_link_libraries(tflogger_python tflogger::tflogger ${Python_LIBRARIES})

install(DIRECTORY ${PY_PACKAGE_DIR} DESTINATION ${Python_SITEARCH})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/tflogger.py DESTINATION ${Python_SITEARCH}/tflogger)
install(TARGETS _tflogger_python DESTINATION ${Python_SITEARCH}/tflogger)
