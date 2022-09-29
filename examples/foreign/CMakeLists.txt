add_executable(cgraph_write_dot dot.c
                      ../../tests/base/gb.c)
target_link_libraries(cgraph_write_dot bkcgraph)