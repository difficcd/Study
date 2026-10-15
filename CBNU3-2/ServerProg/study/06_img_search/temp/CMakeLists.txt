cmake_minimum_required(VERSION 3.13)

project(imgcmp C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LEPT REQUIRED IMPORTED_TARGET lept)

add_executable(imgcmp imgcmp.c)

target_compile_options(imgcmp PRIVATE -g -pthread)

target_link_libraries(imgcmp PRIVATE PkgConfig::LEPT)
target_link_options(imgcmp PRIVATE -pthread)

