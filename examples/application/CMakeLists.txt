add_executable(task1c task1.c)
target_link_libraries(task1c bkcgen bkcgraph)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")
add_executable(task1cc task1.cc)
target_link_libraries(task1cc bkcgen bkcgraph)