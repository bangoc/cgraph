#include "cgraph_error.h"
#include "cgraph_interface.h"

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
    return (CGRAPH_INTEGER) cvector_size(&graph->from);
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
 *          integer number is expected.
 * \param directed Boolean; whether the graph is directed or not. Supported
 *        values are:
 *        \clist
 *        \cli IGRAPH_DIRECTED
 *          The graph will be \em directed.
 *        \cli IGRAPH_UNDIRECTED
 *          The graph will be \em undirected.
 *        \endclist
 * \return Error code:
 *         \c IGRAPH_EINVAL: invalid number of vertices.
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
    graph->from = cvector_create_empty();
    graph->to = cvector_create_empty();
    graph->oi = cvector_create_empty();
    graph->ii = cvector_create_empty();
    graph->os = cvector_create_empty();
    graph->is = cvector_create_empty();

    cvector_push_back(graph->os, 0);
    cvector_push_back(graph->is, 0);

    /* add the vertices */
    CGRAPH_CHECK(cgraph_add_vertices(graph, n));

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
 *           vertices to add.
 * \return Error code:
 *         \c IGRAPH_EINVAL: invalid number of new
 *         vertices.
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

    CGRAPH_CHECK(cgraph_ivec_grow(graph->os, graph->n + nv + 1));
    CGRAPH_CHECK(cgraph_ivec_grow(graph->is, graph->n + nv + 1));
    cgraph_ivec_setsize(graph->os, graph->n + nv + 1);
    cgraph_ivec_setsize(graph->is, graph->n + nv + 1);

    for (i = graph->n + 1; i < graph->n + nv + 1; i++) {
        (graph->os)[i] = ec;
        (graph->is)[i] = ec;
    }

    graph->n += nv;

    return 0;
}