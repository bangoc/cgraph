project (cgraph C)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_C_STANDARD 99)

include_directories(${CMAKE_SOURCE_DIR})
add_subdirectory(examples)
add_subdirectory(tests)