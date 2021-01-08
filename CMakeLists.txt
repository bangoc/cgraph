cmake_minimum_required(VERSION 2.8)
project(cgraph)

option(WARN_ERROR "Turn all warnings into errors" OFF)

set(CMAKE_C_STANDARD 99)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
endif (NOT CMAKE_BUILD_TYPE)

if (${CMAKE_BUILD_TYPE} STREQUAL Debug)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall")
endif()

if (${CMAKE_BUILD_TYPE} STREQUAL Release)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
endif()

if (WARN_ERROR)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")
endif ()

include_directories(${CMAKE_SOURCE_DIR})
add_subdirectory(examples)
add_subdirectory(ctc)

if (${CMAKE_PROJECT_NAME} STREQUAL cgraph)
  include(CTest)
endif()

set(cgraph_src
  cgraph_error.c
  cgraph_ivec.c
  type_indexededgelist.c
  structure_generators.c
  visitors.c
  cgraph_iqueue.c
  cgraph_queue.c
  structural_properties.c
  cgraph_istack.c
  cgraph_rvec.c
  heap.c
)

set(cgraph_inc
  cgraph.h
  cgraph_basic_types.h
  cgraph_constants.h
  cgraph_constructors.h
  cgraph_datatype.h
  cgraph_error.h
  cgraph_interface.h
  cgraph_iqueue.h
  cgraph_istack.h
  cgraph_ivec.h
  cgraph_queue.h
  cgraph_topology.h
  cgraph_visitor.h
  cgraph_paths.h
  cgraph_rvec.h
  cgraph_types_internal.h
)

add_library(cgraph STATIC ${cgraph_src})

if(${BUILD_TESTING})
  add_subdirectory(tests)
  add_test(NAME CvectorCreateEmpty COMMAND cvector_create_empty_ut)
  add_test(NAME CGraphIVecMax COMMAND cgraph_ivec_max_ut)
  add_test(NAME CGraphIvecIsInInterval COMMAND cgraph_ivec_isininterval_ut)
  add_test(NAME CGraphIvecGrowAndSetsize COMMAND cgraph_ivec_grow_and_setsize_ut)
  add_test(NAME CGraphIvecOrder COMMAND cgraph_ivec_order_ut)
  add_test(NAME CreateGraph COMMAND create_graph_ut)
  add_test(NAME CgraphNeighbors COMMAND cgraph_neighbors_ut)
  add_test(NAME CgraphIncident COMMAND cgraph_incident_ut)
  add_test(NAME CgraphIvecAsValue COMMAND cgraph_ivec_as_value_ut)
  add_test(NAME CgraphIvecAsRef COMMAND cgraph_ivec_as_ref_ut)
  add_test(NAME CgraphBfs COMMAND cgraph_bfs_ut)
  add_test(NAME CgraphIQueue COMMAND cgraph_iqueue_ut)
  add_test(NAME CgraphDegree COMMAND cgraph_degree_ut)
  add_test(NAME CgraphIsDag COMMAND cgraph_is_dag_ut)
  add_test(NAME CgraphSimpleBfs COMMAND cgraph_simple_bfs_ut)
  add_test(NAME CgraphIStack COMMAND cgraph_istack_ut)
  add_test(NAME CgraphDfs COMMAND cgraph_dfs_ut)
  add_test(NAME CgraphTopologicalSorting COMMAND cgraph_topological_sorting_ut)
  add_test(NAME CgraphSingleShortestPathDijkstra COMMAND cgraph_single_shortest_path_dijkstra_ut)
  add_test(NAME Cgraph2wHeap COMMAND cgraph_2wheap_ut)
  add_test(NAME CgraphEdge COMMAND cgraph_edge_ut)
  add_test(NAME CgraphGetEid COMMAND cgraph_get_eid_ut)
  add_test(NAME CgraphIvecAppend COMMAND cgraph_ivec_append_ut)
  add_test(NAME CgraphIvecFromArray COMMAND cgraph_ivec_from_array_ut)
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

file (READ ${CMAKE_SOURCE_DIR}/VERSION CGRAPH_VERSION)

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