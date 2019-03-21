cmake_minimum_required(VERSION 3.1)

project(assign_ptr CXX)

option(BUILD_TESTING "" OFF)

add_subdirectory(src)
add_subdirectory(test)

if(BUILD_TESTING)
    enable_testing()
endif(BUILD_TESTING)
