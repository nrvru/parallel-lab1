cmake_minimum_required(VERSION 3.3)
project(parallel_lab1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(OpenMP)
if(OPENMP_FOUND)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

set(SOURCE_FILES main.cpp RandomVector.cpp RandomVector.h SquareMatrix.cpp SquareMatrix.h)
add_executable(parallel_lab1 ${SOURCE_FILES})