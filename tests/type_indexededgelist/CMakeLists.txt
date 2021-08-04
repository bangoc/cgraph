add_executable(cgraph_neighbors_ut test_cgraph_neighbors.c)
add_executable(cgraph_incident_ut test_cgraph_incident.c)
add_executable(cgraph_degree_ut test_cgraph_degree.c)
add_executable(cgraph_edge_ut test_cgraph_edge.c)
add_executable(cgraph_get_eid_ut test_cgraph_get_eid.c)
add_executable(cgraph_add_edges_ut test_cgraph_add_edges.c)
add_executable(cgraph_add_vertices_ut test_cgraph_add_vertices.c)
add_executable(cgraph_delete_edges_ut test_cgraph_delete_edges.c)
foreach(t cgraph_neighbors_ut cgraph_incident_ut cgraph_degree_ut
          cgraph_edge_ut cgraph_get_eid_ut cgraph_add_edges_ut
          cgraph_add_vertices_ut cgraph_delete_edges_ut)
  target_link_libraries(${t} ut_base cgraph)
endforeach()
