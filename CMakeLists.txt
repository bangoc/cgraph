cmake_minimum_required(VERSION 2.8)
project(cgraph)

option(WARN_ERROR "Turn all warnings into errors" OFF)

set(CMAKE_C_STANDARD 11)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif (NOT CMAKE_BUILD_TYPE)

if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
endif()

if (${CMAKE_BUILD_TYPE} STREQUAL Release)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-function")

if (WARN_ERROR)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")
endif ()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/cgen)
add_subdirectory(examples)
add_subdirectory(cgen)

if (${CMAKE_PROJECT_NAME} STREQUAL cgraph)
  include(CTest)
endif()

set(cgraph_src
  base/cgraph_error.c
  base/cgraph_arr.c
  type_indexededgelist.c
  structure_generators.c
  visitors.c
  structural_properties.c
  foreign.c
)

set(cgraph_inc
  cgraph.h
  base/cgraph_basic_types.h
  base/cgraph_constants.h
  cgraph_constructors.h
  base/cgraph_datatype.h
  base/cgraph_error.h
  cgraph_interface.h
  base/cgraph_arr.h
  cgraph_topology.h
  cgraph_visitor.h
  cgraph_paths.h
  cgraph_foreigh.h
)

add_library(cgraph STATIC ${cgraph_src})
target_link_libraries(cgraph bkcgen)

if(${BUILD_TESTING})
  add_subdirectory(tests)
  add_test(NAME CreateGraph COMMAND create_graph_ut)
  add_test(NAME CgraphNeighbors COMMAND cgraph_neighbors_ut)
  add_test(NAME CgraphIncident COMMAND cgraph_incident_ut)
  add_test(NAME CgraphBfs COMMAND cgraph_bfs_ut)
  add_test(NAME CgraphDegree COMMAND cgraph_degree_ut)
  add_test(NAME CgraphIsDag COMMAND cgraph_is_dag_ut)
  add_test(NAME CgraphDfs COMMAND cgraph_dfs_ut)
  add_test(NAME CgraphTopologicalSorting COMMAND cgraph_topological_sorting_ut)
  add_test(NAME CgraphSingleShortestPathDijkstra COMMAND
                cgraph_single_shortest_path_dijkstra_ut)
  add_test(NAME CgraphEdge COMMAND cgraph_edge_ut)
  add_test(NAME CgraphGetEid COMMAND cgraph_get_eid_ut)
  add_test(NAME CgraphMultiShortestPathDijkstra COMMAND
                cgraph_multi_shortest_paths_dijkstra_ut)
  add_test(NAME CgraphGetShortestPaths COMMAND
                cgraph_get_shortest_paths_ut)
  add_test(NAME CgraphAddEdges COMMAND cgraph_add_edges_ut)
  add_test(NAME CgraphAddVertices COMMAND cgraph_add_vertices_ut)
  add_test(NAME CgraphDeleteEdges COMMAND cgraph_delete_edges_ut)
  add_test(NAME CgraphDisconnectVertices COMMAND
                cgraph_disconnect_vertices_ut)
endif()

# e.g. copy libries headers and binary to bkc
# cmake .. -DCMAKE_INSTALL_PREFIX=bkc
MESSAGE(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
set(cgraph_include_dest "include/bkc")
set(cgraph_lib_dest "lib")
install(FILES ${cgraph_inc} DESTINATION "${cgraph_include_dest}")
install(TARGETS cgraph DESTINATION "${cgraph_lib_dest}")

set(CPACK_PACKAGE_INSTALL_DIRECTORY "/usr")
set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Nguyen Ba Ngoc")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION
              "A library of graph algorithms built on top of C \
              standard libraries.")
set(CMAKE_PROJECT_HOMEPAGE_URL "https://github.com/bangoc/cgraph")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "standard")
set(CPACK_DEBIAN_PACKAGE_SECTION "devel")

file (READ ${CMAKE_CURRENT_SOURCE_DIR}/VERSION CGRAPH_VERSION)

# Set a Package Version
set(CPACK_PACKAGE_VERSION ${CGRAPH_VERSION})
include(CPack)

# To build a debian package
# cmake .. -DCMAKE_BUILD_TYPE=Release
# make -j 2
# make package
#
# To install the debian package
# sudo dpkg -i package.deb