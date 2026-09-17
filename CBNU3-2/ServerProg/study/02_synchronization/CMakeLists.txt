cmake_minimum_required(VERSION 3.10)
project(MyProgram C)

set(CMAKE_C_STANDARD 99)

find_package(Threads REQUIRED)
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(prac prac_semaphore.c)
target_link_libraries(prac PRIVATE ${CURSES_LIBRARIES} Threads::Threads)
