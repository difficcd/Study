cmake_minimum_required(VERSION 3.10)
project(MyProgram C)

# C 표준 설정 (선택사항)
set(CMAKE_C_STANDARD 99)

# 1. pthread (쓰레드) 라이브러리 찾기
find_package(Threads REQUIRED)

# 2. ncurses (터미널 UI) 라이브러리 찾기
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

# 3. 실행 파일 생성 (main.c 빌드)
add_executable(my_program mutex.c)

# 4. ncurses 및 pthread 자동 링크 (-lncurses -lpthread 효과)
target_link_libraries(my_program PRIVATE ${CURSES_LIBRARIES} Threads::Threads)
