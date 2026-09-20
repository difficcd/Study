cmake_minimum_required(VERSION 3.10)
project(MultiDirLibExample C)

# 1. 여러 디렉터리에 분포된 소스 파일들을 하나의 변수에 모음
set(LIB_SOURCES
    src/add_dir/add.c
    src/sub_dir/sub.c
)

# 2. 소스 파일들을 묶어서 'mymath'라는 정적 라이브러리(libmymath.a) 생성
add_library(mymath STATIC ${LIB_SOURCES})

# 3. 라이브러리가 사용할 헤더 파일 경로 지정
target_include_directories(mymath PUBLIC include)

# 4. 메인 실행 파일(main) 생성
add_executable(my_app main.c)

# 5. 실행 파일에 정적 라이브러리(libmymath.a) 링크
target_link_libraries(my_app PRIVATE mymath)
