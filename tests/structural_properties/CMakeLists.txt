add_executable(cgraph_is_dag_ut test_cgraph_is_dag.c)
add_executable(cgraph_topological_sorting_ut
               test_cgraph_topological_sorting.c)
add_executable(cgraph_single_shortest_path_dijkstra_ut
              test_cgraph_single_shortest_path_dijkstra.c)
add_executable(cgraph_multi_shortest_paths_dijkstra_ut
              test_cgraph_multi_shortest_paths_dijkstra.c)
foreach (t cgraph_is_dag_ut
           cgraph_topological_sorting_ut
           cgraph_single_shortest_path_dijkstra_ut
           cgraph_multi_shortest_paths_dijkstra_ut)
  target_link_libraries(${t} ut_base cgraph)
endforeach()