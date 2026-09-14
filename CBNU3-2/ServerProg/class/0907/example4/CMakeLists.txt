cmake_minimum_required(VERSION 3.20)

project(hello C)

set(CURSES_USE_NCURSES TRUE)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_subdirectory(lib)
add_subdirectory(src)
