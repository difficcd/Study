add_library(print_msg print.c)
target_include_directories(print_msg PUBLIC
	${PROJECT_SOURCE_DIR}/include)
