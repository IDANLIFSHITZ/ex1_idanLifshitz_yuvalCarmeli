cmake_minimum_required(VERSION 3.24)
project(ex1_idanLifshitz_yuvalCarmeli C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(ex1_idanLifshitz_yuvalCarmeli
        IsraeliQueue.c
        IsraeliQueue.h)
