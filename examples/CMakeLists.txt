add_executable(create_graph_demo create_graph_demo.c)
add_executable(cgraph_set_error_handler_demo cgraph_set_error_handler_demo.c)

foreach(t create_graph_demo cgraph_set_error_handler_demo)
  target_link_libraries(${t} cgraph)
endforeach()

add_subdirectory(application)
add_subdirectory(cgen)
add_subdirectory(bus)
add_subdirectory(foreign)