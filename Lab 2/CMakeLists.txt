cmake_minimum_required(VERSION 3.20)
project(Lab_2 C)

set(CMAKE_C_STANDARD 99)

add_executable(recognition mainRecog.c recognizeEq.c recognizeEq.h scanner.c scanner.h)
