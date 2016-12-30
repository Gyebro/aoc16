cmake_minimum_required(VERSION 3.6)
project(aoc16)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp md5.h md5.cpp)
add_executable(aoc16 ${SOURCE_FILES})