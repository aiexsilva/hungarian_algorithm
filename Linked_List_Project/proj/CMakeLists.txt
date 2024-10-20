cmake_minimum_required(VERSION 3.27)
project(proj C)

set(CMAKE_C_STANDARD 11)

add_executable(proj main.c
        cmake-build-debug/hungarian_algorithm_explained.h)
