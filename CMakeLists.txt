cmake_minimum_required(VERSION 3.8)

project(TauCompiler)

option(COMPILE_TESTS        "Execute compile-time tests"     ON)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-deprecated-register -Wno-unused-function")

include(${CMAKE_SOURCE_DIR}/cmake/FindCppuTest.cmake)

find_package(libnabla REQUIRED)

include_directories(${LIBNABLA_INCLUDE_DIRS})

include_directories(${CMAKE_SOURCE_DIR}/parser)

set(PARSER_SOURCES
    ${CMAKE_SOURCE_DIR}/parser/Parser.cpp
    ${CMAKE_SOURCE_DIR}/parser/TokenStream.cpp
)

add_executable(tau
        ${PARSER_SOURCES}
        main.cpp
)

target_link_libraries(tau
    PRIVATE
        ${LIBNABLA_LIBRARIES}
)