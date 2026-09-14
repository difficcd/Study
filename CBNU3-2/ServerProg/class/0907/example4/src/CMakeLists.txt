add_executable(hello main.c)
target_link_libraries(hello print_msg)
target_link_libraries(hello ${CURSES_LIBRARY})
set_target_properties(hello PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
)
