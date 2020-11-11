cmake_minimum_required(VERSION 3.19.0)

project (cgraph C)

add_library(
	cgraph_lib
	cgraph_constants.h
	cgraph_constructors.h cgraph_constructors.c
	cgraph_datatype.h
	cgraph_error.h cgraph_error.c
	cgraph_interface.h cgraph_interface.c
	cgraph.h
	cvector.h
)

target_include_directories(cgraph_lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(examples)