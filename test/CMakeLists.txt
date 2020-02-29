include(ExternalProject)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(tfloggerTest
  event_logger_test.cc
  record_test.cc
  tflogger_test.cc
  )

if(NOT TARGET Catch2::Catch)
  message(STATUS "Catch will be downloaded when ${CMAKE_PROJECT_NAME} is built")
  ExternalProject_Add(catch-lib
        PREFIX ${THIRD_PARTY_PATH}/Catch
        #--Download step--------------
        URL https://github.com/catchorg/Catch2/archive/v2.11.1.zip
        TIMEOUT 30
        #--Update/Patch step----------
        UPDATE_COMMAND ""
        PATCH_COMMAND ""
        #--Configure step-------------
        CONFIGURE_COMMAND ""
        #--Build step-----------------
        BUILD_COMMAND ""
        #--Install step---------------
        INSTALL_COMMAND ""
        #--Output logging-------------
        LOG_DOWNLOAD ON
  )
  add_dependencies(tfloggerTest catch-lib)
  ExternalProject_Get_Property(catch-lib source_dir)
  set(CATCH_INCLUDE_DIRS ${source_dir}/single_include CACHE INTERNAL "Path to include folder for Catch")
  add_library(Catch2::Catch INTERFACE IMPORTED)
else()
  get_target_property(CATCH_INCLUDE_DIRS Catch2::Catch INTERFACE_INCLUDE_DIRECTORIES)
  message(STATUS "Using Catch v${Catch2_VERSION} from: ${CATCH_INCLUDE_DIRS}")
endif()

target_include_directories(tfloggerTest PRIVATE ${CATCH_INCLUDE_DIRS})
target_include_directories(tfloggerTest PRIVATE ${CMAKE_SOURCE_DIR})
target_link_libraries(tfloggerTest PRIVATE tflogger)
target_link_libraries(tfloggerTest PRIVATE Catch2::Catch)

add_test(NAME tfloggerTest COMMAND "${CMAKE_CURRENT_BINARY_DIR}/tfloggerTest")
