cmake_minimum_required(VERSION 3.20)

project(hello C)

add_library(print_msg print.c)

add_executable(hello main.c)

target_link_libraries(hello print_msg)
