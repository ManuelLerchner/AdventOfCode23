cmake_minimum_required(VERSION 3.15)

project(AdventOfCode2023)

# load all files in each folder
file(GLOB_RECURSE SOURCES "day*/*.cpp" "day*/*.h")
file(GLOB_RECURSE UTIL_SOURCES "util/*.cpp" "util/*.h")

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_compile_definitions(ADVENT_OF_CODE_DATA_DIR="${CMAKE_SOURCE_DIR}/data/")

include_directories(${CMAKE_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 20)

foreach(SOURCE ${SOURCES})
    get_filename_component(DAY ${SOURCE} DIRECTORY)
    get_filename_component(DAY ${DAY} NAME)

    add_executable(${DAY} ${SOURCE} ${UTIL_SOURCES})
endforeach()