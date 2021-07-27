add_executable(cgraph_write_dot dot.c
                      ${CMAKE_SOURCE_DIR}/tests/base/gb.c)
target_link_libraries(cgraph_write_dot cgraph)