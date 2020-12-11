#include "cgraph_constants.h"
#include "cgraph_datatype.h"
#include "cgraph_error.h"
#include "cgraph_interface.h"
#include "cgraph_iqueue.h"
#include "cgraph_ivec.h"
#include "cgraph_paths.h"
#include "cgraph_topology.h"

int cgraph_is_dag(const cgraph_t *graph, bool *res) {
  if (!cgraph_is_directed(graph)) {
    *res = false;
    return 0;
  }

  CGRAPH_INTEGER no_of_nodes = cgraph_vcount(graph);
  cgraph_ivec_t degrees = cgraph_ivec_create(),
                neis = cgraph_ivec_create();
  cgraph_iqueue_t sources = cgraph_iqueue_create();
  CGRAPH_INTEGER node, i, j, nei, vertices_left;

  cgraph_degree_all(graph, &degrees, CGRAPH_OUT, true);

  vertices_left = no_of_nodes;

  /* Do we have nodes with no incoming edges? */
  for (i = 0; i < no_of_nodes; i++) {
    if (degrees[i] == 0) {
      CGRAPH_CHECK(cgraph_iqueue_enqueue(sources, i));
    }
  }

  /* Take all nodes with no incoming edges and remove them */
  while (!cgraph_iqueue_empty(sources)) {
    cgraph_iqueue_poll(sources, &node);
    /* Exclude the node from further source searches */
    degrees[node] = -1;
    vertices_left--;

    /* Get the neighbors and decrease their degrees by one */
    CGRAPH_CHECK(cgraph_neighbors(graph, &neis, node, CGRAPH_IN));
    j = cgraph_ivec_size(neis);
    for (i = 0; i < j; i++) {
      nei = neis[i];
      // if (nei == node) {
      //   continue;
      // }
      degrees[nei]--;
      if (degrees[nei] == 0) {
        CGRAPH_CHECK(cgraph_iqueue_enqueue(sources, nei));
      }
    }
  }

  *res = (vertices_left == 0);
  if (vertices_left < 0) {
    CGRAPH_ERROR("vertices_left < 0 in cgraph_is_dag, possible bug");
  }

  cgraph_ivec_free(&degrees);
  cgraph_ivec_free(&neis);
  cgraph_iqueue_free(&sources);

  return 0;
}

/**
 * \function cgraph_topological_sorting
 * \brief Calculate a possible topological sorting of the graph.
 *
 * </para><para>
 * A topological sorting of a directed acyclic graph is a linear ordering
 * of its nodes where each node comes before all nodes to which it has
 * edges. Every DAG has at least one topological sort, and may have many.
 * This function returns a possible topological sort among them. If the
 * graph is not acyclic (it has at least one cycle), a partial topological
 * sort is returned and a warning is issued.
 *
 * \param graph The input graph.
 * \param res Pointer to a vector, the result will be stored here.
 *   It will be resized if needed.
 * \param mode Specifies how to use the direction of the edges.
 *   For \c IGRAPH_OUT, the sorting order ensures that each node comes
 *   before all nodes to which it has edges, so nodes with no incoming
 *   edges go first. For \c IGRAPH_IN, it is quite the opposite: each
 *   node comes before all nodes from which it receives edges. Nodes
 *   with no outgoing edges go first.
 * \return Error code.
 *
 * Time complexity: O(|V|+|E|), where |V| and |E| are the number of
 * vertices and edges in the original input graph.
 *
 * \sa \ref igraph_is_dag() if you are only interested in whether a given
 *     graph is a DAG or not, or \ref igraph_feedback_arc_set() to find a
 *     set of edges whose removal makes the graph a DAG.
 *
 * \example examples/simple/igraph_topological_sorting.c
 */
int cgraph_topological_sorting(const cgraph_t *graph,
                               cgraph_ivec_t *res,
                               cgraph_neimode_t mode) {
  CGRAPH_INTEGER no_of_nodes = cgraph_vcount(graph);
  cgraph_ivec_t degrees = cgraph_ivec_create(),
                neis = cgraph_ivec_create();
  cgraph_iqueue_t sources = cgraph_iqueue_create();
  cgraph_neimode_t deg_mode;

  if (mode == CGRAPH_ALL || !cgraph_is_directed(graph)) {
    CGRAPH_ERROR("topological sorting does not make sense for undirected graphs");
  } else if (mode == CGRAPH_OUT) {
    deg_mode = CGRAPH_IN;
  } else if (mode == CGRAPH_IN) {
    deg_mode = CGRAPH_OUT;
  } else {
    CGRAPH_ERROR("invalid mode");
  }

  /* with loops, igraph doesn't count loop */
  CGRAPH_CHECK(cgraph_degree_all(graph, &degrees, deg_mode, 1));

  cgraph_ivec_setsize(*res, 0);
  /* Do we have nodes with no incoming vertices? */
  for (CGRAPH_INTEGER i = 0; i < no_of_nodes; i++) {
    if (degrees[i] == 0) {
      CGRAPH_CHECK(cgraph_iqueue_enqueue(sources, i));
    }
  }

  /* Take all nodes with no incoming vertices and remove them */
  while (!cgraph_iqueue_empty(sources)) {
    CGRAPH_INTEGER node;
    cgraph_iqueue_poll(sources, &node);
    cgraph_ivec_push_back(res, node);
    degrees[node] = -1;
    CGRAPH_CHECK(cgraph_neighbors(graph, &neis, node, mode));
    for (CGRAPH_INTEGER i = 0; i < cgraph_ivec_size(neis); i++) {
      degrees[neis[i]]--;
      if (degrees[neis[i]] == 0) {
        CGRAPH_CHECK(cgraph_iqueue_enqueue(sources, neis[i]));
      }
    }
  }

  if (cgraph_ivec_size(*res) < no_of_nodes) {
    CGRAPH_ERROR("graph contains a cycle, partial result is returned");
  }

  cgraph_ivec_free(&degrees);
  cgraph_ivec_free(&neis);
  cgraph_iqueue_free(&sources);
  return 0;
}

/**
 * \function  cgraph_get_shortest_path_dijkstra
 *            igraph_get_shortest_path_dijkstra
 * Weighted shortest path from one vertex to another one.
 *
 * Calculates a single (positively) weighted shortest path from
 * a single vertex to another one, using Dijkstra's algorithm.
 *
 * </para><para>This function is a special case (and a wrapper) to
 * \ref igraph_get_shortest_paths_dijkstra().
 *
 * \param graph The input graph, it can be directed or undirected.
 * \param vertices Pointer to an initialized vector or a null
 *        pointer. If not a null pointer, then the vertex ids along
 *        the path are stored here, including the source and target
 *        vertices.
 * \param edges Pointer to an uninitialized vector or a null
 *        pointer. If not a null pointer, then the edge ids along the
 *        path are stored here.
 * \param from The id of the source vertex.
 * \param to The id of the target vertex.
 * \param weights Vector of edge weights, in the order of edge
 *        ids. They must be non-negative, otherwise the algorithm does
 *        not work.
 * \param mode A constant specifying how edge directions are
 *        considered in directed graphs. \c IGRAPH_OUT follows edge
 *        directions, \c IGRAPH_IN follows the opposite directions,
 *        and \c IGRAPH_ALL ignores edge directions. This argument is
 *        ignored for undirected graphs.
 * \return Error code.
 *
 * Time complexity: O(|E|log|E|+|V|), |V| is the number of vertices,
 * |E| is the number of edges in the graph.
 *
 * \sa \ref igraph_get_shortest_paths_dijkstra() for the version with
 * more target vertices.
 */

int cgraph_get_shortest_path_dijkstra(const cgraph_t *graph,
        cgraph_ivec_t *vertices,
        cgraph_ivec_t *edges,
        CGRAPH_INTEGER from,
        CGRAPH_INTEGER to,
        const double *weights,
        cgraph_neimode_t mode) {
  /*
    TODO: Implementation
  */
  return 0;
}