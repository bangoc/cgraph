#include "cgraph_constructors.h"
#include "cgraph_error.h"
#include "cgraph_interface.h"
#include "cgraph_ivec.h"

#include <stdlib.h>

/**
 * \ingroup interface
 * \function igraph_empty
 * \brief Creates an empty graph with some vertices and no edges.
 *
 * </para><para>
 * The most basic constructor, all the other constructors should call
 * this to create a minimal graph object. Our use of the term "empty graph"
 * in the above description should be distinguished from the mathematical
 * definition of the empty or null graph. Strictly speaking, the empty or null
 * graph in graph theory is the graph with no vertices and no edges. However
 * by "empty graph" as used in \c igraph we mean a graph having zero or more
 * vertices, but no edges.
 * \param graph Pointer to a not-yet initialized graph object.
 * \param n The number of vertices in the graph, a non-negative
 *      integer number is expected.
 * \param directed Boolean; whether the graph is directed or not. Supported
 *    values are:
 *    \clist
 *    \cli IGRAPH_DIRECTED
 *      The graph will be \em directed.
 *    \cli IGRAPH_UNDIRECTED
 *      The graph will be \em undirected.
 *    \endclist
 * \return Error code:
 *     \c IGRAPH_EINVAL: invalid number of vertices.
 *
 * Time complexity: O(|V|) for a graph with
 * |V| vertices (and no edges).
 *
 * \example examples/simple/igraph_empty.c
 */

cgraph_p cgraph_create_empty(CGRAPH_INTEGER n, bool directed) {

  if (n < 0) {
    CGRAPH_ERROR("cannot create empty graph with negative number of vertices", CGRAPH_FAILURE);
    return NULL;
  }

  cgraph_p graph = malloc(sizeof(struct cgraph_s));

  graph->n = 0;
  graph->directed = directed;
  graph->from = cgraph_ivec_create();
  graph->to = cgraph_ivec_create();
  graph->oi = cgraph_ivec_create();
  graph->ii = cgraph_ivec_create();
  graph->os = cgraph_ivec_create();
  graph->is = cgraph_ivec_create();

  cgraph_ivec_push_back(&graph->os, 0);
  cgraph_ivec_push_back(&graph->is, 0);

  /* add the vertices */
  CGRAPH_CHECK(cgraph_add_vertices(graph, n));

  return graph;
}

cgraph_p cgraph_create(cgraph_ivec_t const edges,
          CGRAPH_INTEGER n,
          bool directed) {
  bool has_edges = cgraph_ivec_size(edges) > 0;
  CGRAPH_INTEGER max = has_edges ? cgraph_ivec_max(edges) + 1 : 0;

  if (cgraph_ivec_size(edges) % 2 != 0) {
    CGRAPH_ERROR("Invalid (odd) edges vector", CGRAPH_FAILURE);
  }
  if (has_edges && !cgraph_ivec_isininterval(edges, 0, max - 1)) {
    CGRAPH_ERROR("Invalid (negative) vertex id", CGRAPH_FAILURE);
  }

  cgraph_p graph = cgraph_create_empty(n, directed);

  if (has_edges) {
    CGRAPH_INTEGER vc = cgraph_vcount(graph);
    if (vc < max) {
      CGRAPH_CHECK(cgraph_add_vertices(graph, (CGRAPH_INTEGER) (max - vc)));
    }
    CGRAPH_CHECK(cgraph_add_edges(graph, edges));
  }

  return graph;
}