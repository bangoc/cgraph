#include "cgraph_constructors.h"
#include "cgraph_error.h"
#include "cgraph_interface.h"
#include "cgraph_ivec.h"

int cgraph_create(cgraph_t *graph, 
          cgraph_ivec_t const edges,
          CGRAPH_INTEGER n, 
          bool directed) {
  bool has_edges = cgraph_ivec_size(edges) > 0;
  CGRAPH_INTEGER max = has_edges ? cgraph_ivec_max(edges) + 1 : 0;

  if (cgraph_ivec_size(edges) % 2 != 0) {
    CGRAPH_ERROR("Invalid (odd) edges vector");
  }
  if (has_edges && !cgraph_ivec_isininterval(edges, 0, max - 1)) {
    CGRAPH_ERROR("Invalid (negative) vertex id");
  }

  CGRAPH_CHECK(cgraph_empty(graph, n, directed));
  
  if (has_edges) {
    CGRAPH_INTEGER vc = cgraph_vcount(graph);
    if (vc < max) {
      CGRAPH_CHECK(cgraph_add_vertices(graph, (CGRAPH_INTEGER) (max - vc)));
    }
    CGRAPH_CHECK(cgraph_add_edges(graph, edges));
  }

  return 0;
}