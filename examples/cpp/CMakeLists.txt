add_executable(logger logger.cc)
target_include_directories(logger PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(logger PRIVATE tflogger::tflogger)

add_executable(reader reader.cc)
target_include_directories(reader PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(reader PRIVATE tflogger::tflogger)
