cmake_minimum_required(VERSION 3.8)

project(TauCompiler)

option(COMPILE_TESTS        "Execute compile-time tests"     ON)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-deprecated-register -Wno-unused-function")

include(${CMAKE_SOURCE_DIR}/cmake/FindCppuTest.cmake)

#
#   Libnabla
#
find_package(libnabla REQUIRED)
include_directories(${LIBNABLA_INCLUDE_DIRS})


#
#   Driver
#
set(DRIVER_DIR
    ${CMAKE_SOURCE_DIR}/driver
)

set(DRIVER_SOURCES
    ${DRIVER_DIR}/Driver.cpp
)

#   Reporter
#
set(REPORTER_DIR
    ${CMAKE_SOURCE_DIR}/reporter
)

set(REPORTER_SOURCES
    ${REPORTER_DIR}/Reporter.cpp
)

#   Parser
#
set(PARSER_DIR
    ${CMAKE_SOURCE_DIR}/parser
)

set(PARSER_SOURCES
    ${PARSER_DIR}/Parser.cpp
    ${PARSER_DIR}/TokenStream.cpp
)

#   Lib
#   
add_library(tau-lib 
    ${DRIVER_SOURCES}
    ${REPORTER_SOURCES}
    ${PARSER_SOURCES}
)

#
#   Include directories
#
include_directories(${CMAKE_SOURCE_DIR}/driver)
include_directories(${CMAKE_SOURCE_DIR}/parser)
include_directories(${CMAKE_SOURCE_DIR}/reporter)

#
#   Tests
#
if(${COMPILE_TESTS})
    include_directories(${CMAKE_SOURCE_DIR}/app_tests)
    add_subdirectory(app_tests)
endif()

#
#   Applications
#
add_executable(tau
        main.cpp
)

target_link_libraries(tau
    PRIVATE
        tau-lib
        ${LIBNABLA_LIBRARIES}
)

#   Copy over test code for development
#
#
add_custom_target(copy-dev-targets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/test_code ${CMAKE_BINARY_DIR}/test_code
)
