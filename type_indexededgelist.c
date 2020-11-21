#include "cgraph_error.h"
#include "cgraph_interface.h"
#include "cgraph_vector.h"

/**
 * \ingroup interface
 * \function igraph_vcount
 * \brief The number of vertices in a graph.
 *
 * \param graph The graph.
 * \return Number of vertices.
 *
 * Time complexity: O(1)
 */
;
CGRAPH_INTEGER cgraph_vcount(const cgraph_t *graph) {
  return graph->n;
}

/**
 * \ingroup interface
 * \function igraph_ecount
 * \brief The number of edges in a graph.
 *
 * \param graph The graph.
 * \return Number of edges.
 *
 * Time complexity: O(1)
 */
CGRAPH_INTEGER cgraph_ecount(const cgraph_t *graph) {
  return (CGRAPH_INTEGER) cgraph_ivec_size(graph->from);
}

/**
 * \ingroup interface
 * \function igraph_is_directed
 * \brief Is this a directed graph?
 *
 * \param graph The graph.
 * \return Logical value, <code>TRUE</code> if the graph is directed,
 * <code>FALSE</code> otherwise.
 *
 * Time complexity: O(1)
 *
 * \example examples/simple/igraph_is_directed.c
 */

bool cgraph_is_directed(const cgraph_t *graph) {
    return graph->directed;
}

static int cgraph_i_create_start(
        cgraph_ivec_t res, cgraph_ivec_t el,
        cgraph_ivec_t iindex, CGRAPH_INTEGER nodes) {

# define EDGE(i) el[ iindex[(i)] ]

    CGRAPH_INTEGER no_of_nodes;
    CGRAPH_INTEGER no_of_edges;
    CGRAPH_INTEGER i, j, idx;

    no_of_nodes = nodes;
    no_of_edges = cgraph_ivec_size(el);

    /* result */

    cgraph_ivec_setsize(res, nodes + 1);

    /* create the index */

    if (cgraph_ivec_size(el) == 0) {
        /* empty graph */
        cgraph_ivec_null(res);
    } else {
        idx = -1;
        for (i = 0; i <= EDGE(0); i++) {
            idx++; res[idx] = 0;
        }
        for (i = 1; i < no_of_edges; i++) {
            CGRAPH_INTEGER n = 
              (CGRAPH_INTEGER) (EDGE(i) - EDGE(res[idx]));
            for (j = 0; j < n; j++) {
                idx++; res[idx] = i;
            }
        }
        j = EDGE(res[idx]);
        for (i = 0; i < no_of_nodes - j; i++) {
            idx++; res[idx] = no_of_edges;
        }
    }

    /* clean */

# undef EDGE
    return 0;
}

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

int cgraph_empty(cgraph_t *graph, CGRAPH_INTEGER n, bool directed) {

  if (n < 0) {
    CGRAPH_ERROR("cannot create empty graph with negative number of vertices");
  }

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

  return 0;
}

/**
 * \ingroup interface
 * \function igraph_add_edges
 * \brief Adds edges to a graph object.
 *
 * </para><para>
 * The edges are given in a vector, the
 * first two elements define the first edge (the order is
 * <code>from</code>, <code>to</code> for directed
 * graphs). The vector
 * should contain even number of integer numbers between zero and the
 * number of vertices in the graph minus one (inclusive). If you also
 * want to add new vertices, call igraph_add_vertices() first.
 * \param graph The graph to which the edges will be added.
 * \param edges The edges themselves.
 *
 * This function invalidates all iterators.
 *
 * </para><para>
 * Time complexity: O(|V|+|E|) where
 * |V| is the number of vertices and
 * |E| is the number of
 * edges in the \em new, extended graph.
 *
 * \example examples/simple/igraph_add_edges.c
 */
int cgraph_add_edges(cgraph_t *graph, const cgraph_ivec_t edges) {
    long int no_of_edges = cgraph_ecount(graph);
    long int edges_to_add = cgraph_ivec_size(edges) / 2;
    long int i = 0;
    cgraph_error_handler_t *oldhandler;
    bool ret1, ret2;
    cgraph_ivec_t newoi = cgraph_ivec_create(), 
                  newii = cgraph_ivec_create();
    bool directed = cgraph_is_directed(graph);

    if (cgraph_ivec_size(edges) % 2 != 0) {
        CGRAPH_ERROR("invalid (odd) length of edges vector");
    }
    if (!cgraph_ivec_isininterval(edges, 0, cgraph_vcount(graph) - 1)) {
        CGRAPH_ERROR("cannot add edges");
    }

    /* from & to */
    CGRAPH_CHECK(cgraph_ivec_grow(&graph->from, no_of_edges + edges_to_add));
    CGRAPH_CHECK(cgraph_ivec_grow(&graph->to, no_of_edges + edges_to_add));

    while (i < edges_to_add * 2) {
        if (directed || edges[i] > edges[i + 1]) {
            cgraph_ivec_push_back(&graph->from, edges[i++]); /* reserved */
            cgraph_ivec_push_back(&graph->to,   edges[i++]); /* reserved */
        } else {
            cgraph_ivec_push_back(&graph->to,   edges[i++]); /* reserved */
            cgraph_ivec_push_back(&graph->from, edges[i++]); /* reserved */
        }
    }

    /* disable the error handler temporarily */
    oldhandler = cgraph_set_error_handler(cgraph_error_handler_ignore);

    /* oi & ii */
    ret1 = cgraph_ivec_init(&newoi, no_of_edges + edges_to_add);
    ret2 = cgraph_ivec_init(&newii, no_of_edges + edges_to_add);
    if (ret1 != 0 || ret2 != 0) {
        cgraph_ivec_setsize(graph->from, no_of_edges); /* gets smaller */
        cgraph_ivec_setsize(graph->to, no_of_edges);   /* gets smaller */
        cgraph_set_error_handler(oldhandler);
        CGRAPH_ERROR("cannot add edges");
    }
    ret1 = cgraph_ivec_order(graph->from, graph->to, newoi);
    ret2 = cgraph_ivec_order(graph->to, graph->from, newii);
    if (ret1 != 0 || ret2 != 0) {
        cgraph_ivec_setsize(graph->from, no_of_edges);
        cgraph_ivec_setsize(graph->to, no_of_edges);
        cgraph_ivec_free(&newoi);
        cgraph_ivec_free(&newii);
        cgraph_set_error_handler(oldhandler);
        CGRAPH_ERROR("cannot add edges");
    }

    /* os & is, its length does not change, error safe */
    cgraph_i_create_start(graph->os, graph->from, newoi, graph->n);
    cgraph_i_create_start(graph->is, graph->to, newii, graph->n);

    /* everything went fine  */
    cgraph_ivec_free(&graph->oi);
    cgraph_ivec_free(&graph->ii);
    graph->oi = newoi;
    graph->ii = newii;
    cgraph_set_error_handler(oldhandler);

    return 0;
}

/**
 * \ingroup interface
 * \function igraph_add_vertices
 * \brief Adds vertices to a graph.
 *
 * </para><para>
 * This function invalidates all iterators.
 *
 * \param graph The graph object to extend.
 * \param nv Non-negative integer giving the number of
 *       vertices to add.
 * \return Error code:
 *     \c IGRAPH_EINVAL: invalid number of new
 *     vertices.
 *
 * Time complexity: O(|V|) where
 * |V| is
 * the number of vertices in the \em new, extended graph.
 *
 * \example examples/simple/igraph_add_vertices.c
 */
int cgraph_add_vertices(cgraph_t *graph, CGRAPH_INTEGER nv) {
  long int ec = cgraph_ecount(graph);
  long int i;

  if (nv < 0) {
    CGRAPH_ERROR("cannot add negative number of vertices");
  }

  CGRAPH_CHECK(cgraph_ivec_grow(&graph->os, graph->n + nv + 1));
  CGRAPH_CHECK(cgraph_ivec_grow(&graph->is, graph->n + nv + 1));
  cgraph_ivec_setsize(graph->os, graph->n + nv + 1);
  cgraph_ivec_setsize(graph->is, graph->n + nv + 1);

  for (i = graph->n + 1; i < graph->n + nv + 1; i++) {
    (graph->os)[i] = ec;
    (graph->is)[i] = ec;
  }

  graph->n += nv;

  return 0;
}

/**
 * \ingroup interface
 * \function igraph_destroy
 * \brief Frees the memory allocated for a graph object.
 *
 * </para><para>
 * This function should be called for every graph object exactly once.
 *
 * </para><para>
 * This function invalidates all iterators (of course), but the
 * iterators of a graph should be destroyed before the graph itself
 * anyway.
 * \param graph Pointer to the graph to free.
 *
 * Time complexity: operating system specific.
 */
void cgraph_destroy(cgraph_t *graph) {
    cgraph_ivec_free(&graph->from);
    cgraph_ivec_free(&graph->to);
    cgraph_ivec_free(&graph->oi);
    cgraph_ivec_free(&graph->ii);
    cgraph_ivec_free(&graph->os);
    cgraph_ivec_free(&graph->is);
}

int cgraph_neighbors(const cgraph_t *graph, 
                     cgraph_ivec_t *neis, 
                     CGRAPH_INTEGER vid,
                     cgraph_neimode_t mode) {
  /*
    TODO: Complete this function to pass the CgraphNeighbors
  */
  return 0;
}

int cgraph_incident(const cgraph_t *graph, 
                    cgraph_ivec_t *eids, 
                    CGRAPH_INTEGER vid,
                    cgraph_neimode_t mode) {
  const CGRAPH_INTEGER node = vid;
  if (node < 0 || node > cgraph_vcount(graph) - 1) {
    CGRAPH_ERROR("cannot get neighbors");
  }
  if (mode != CGRAPH_OUT && mode != CGRAPH_IN &&
      mode != CGRAPH_ALL) {
    CGRAPH_ERROR("cannot get neighbors");
  }

  if (! graph->directed) {
    mode = CGRAPH_ALL;
  }

  cgraph_ivec_setsize(*eids, 0);

  if (mode & CGRAPH_OUT) {
    CGRAPH_INTEGER j = (graph->os)[node + 1];
    for (CGRAPH_INTEGER i = (graph->os)[node]; i < j; i++) {
      cgraph_ivec_push_back(eids, (graph->oi)[i]);
    }
  }
  if (mode & CGRAPH_IN) {
    CGRAPH_INTEGER j = (graph->is)[node + 1];
    for (CGRAPH_INTEGER i = (graph->is)[node]; i < j; i++) {
      cgraph_ivec_push_back(eids, (graph->ii)[i]);
    }
  }
  return 0;
}
