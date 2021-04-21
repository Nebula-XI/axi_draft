cmake_minimum_required(VERSION 3.16)

project(axi_draft
	DESCRIPTION "axi_draft Project"
)

add_subdirectory(tests/info)
add_subdirectory(tests/dev)

enable_testing()
