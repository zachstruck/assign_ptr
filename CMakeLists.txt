cmake_minimum_required(VERSION 3.0)

project(assign_ptr CXX)

option(BUILD_TESTING "" OFF)
include(CTest)

add_subdirectory(src)
add_subdirectory(test)
