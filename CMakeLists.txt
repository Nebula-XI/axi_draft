cmake_minimum_required(VERSION 3.16)

project(axi_draft
	DESCRIPTION "axi_draft Project"
)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

enable_testing()

add_subdirectory(tests/info)
add_subdirectory(tests/dev)
