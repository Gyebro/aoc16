cmake_minimum_required(VERSION 3.6)
project(aoc16)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp md5.h md5.cpp Dexel2.h dexel2.cpp)
add_executable(aoc16 ${SOURCE_FILES})

add_custom_command(TARGET aoc16 PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/input $<TARGET_FILE_DIR:aoc16>)