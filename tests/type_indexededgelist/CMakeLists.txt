add_executable(cgraph_neighbors_ut test_cgraph_neighbors.c)
add_executable(cgraph_incident_ut test_cgraph_incident.c)
add_executable(cgraph_degree_ut test_cgraph_degree.c)
add_executable(cgraph_edge_ut test_cgraph_edge.c)
add_executable(cgraph_get_eid_ut test_cgraph_get_eid.c)
foreach(t cgraph_neighbors_ut cgraph_incident_ut cgraph_degree_ut
          cgraph_edge_ut cgraph_get_eid_ut)
  target_link_libraries(${t} ut_base cgraph)
endforeach()
