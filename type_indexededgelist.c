#include "cgraph_error.h"
#include "cgraph_interface.h"
#include "cgraph_ivec.h"

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
int cgraph_add_edges(cgraph_t *graph, cgraph_ivec_t const edges) {
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

  cgraph_ivec_setsize(*neis, 0);

  if (!cgraph_is_directed(graph) || mode != CGRAPH_ALL) {
    if (mode & CGRAPH_OUT) {
      CGRAPH_INTEGER j = (graph->os)[node + 1];
      for (CGRAPH_INTEGER i = (graph->os)[node]; i < j; i++) {
        cgraph_ivec_push_back(neis, (graph->to)[ (graph->oi)[i] ]);
      }
    }
    if (mode & CGRAPH_IN) {
      CGRAPH_INTEGER j = (graph->is)[node + 1];
      for (CGRAPH_INTEGER i = (graph->is)[node]; i < j; i++) {
        cgraph_ivec_push_back(neis, (graph->from)[ (graph->ii)[i] ]);
      }
    }
  } else {
    /* both in- and out- neighbors in a directed graph,
       we need to merge the two 'vectors' */
    CGRAPH_INTEGER j1 = (graph->os)[node + 1];
    CGRAPH_INTEGER j2 = (graph->is)[node + 1];
    CGRAPH_INTEGER i1 = (graph->os)[node];
    CGRAPH_INTEGER i2 = (graph->is)[node];
    while (i1 < j1 && i2 < j2) {
      CGRAPH_INTEGER n1 = (graph->to)[ (graph->oi)[i1] ];
      CGRAPH_INTEGER n2 = (graph->from)[ (graph->ii)[i2] ];
      if (n1 < n2) {
        cgraph_ivec_push_back(neis, n1);
        i1++;
      } else if (n1 > n2) {
        cgraph_ivec_push_back(neis, n2);
        i2++;
      } else {
        cgraph_ivec_push_back(neis, n1);
        cgraph_ivec_push_back(neis, n2);
        i1++;
        i2++;
      }
    }
    while (i1 < j1) {
      CGRAPH_INTEGER n1 = (graph->to)[ (graph->oi)[i1] ];
      cgraph_ivec_push_back(neis, n1);
      i1++;
    }
    while (i2 < j2) {
      CGRAPH_INTEGER n2 = (graph->from)[ (graph->ii)[i2] ];
      cgraph_ivec_push_back(neis, n2);
      i2++;
    }
  }

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

int cgraph_degree_all(const cgraph_t *graph,
                      cgraph_ivec_t *res,
                      cgraph_neimode_t mode,
                      bool loops) {
  if (mode != CGRAPH_OUT && mode != CGRAPH_IN &&
      mode != CGRAPH_ALL) {
    CGRAPH_ERROR("cannot get degree");
  }
  if (!cgraph_is_directed(graph)) {
    mode = CGRAPH_ALL;
  }
  CGRAPH_INTEGER no_of_nodes = cgraph_vcount(graph);
  CGRAPH_INTEGER no_of_edges = cgraph_ecount(graph);
  cgraph_ivec_init(res, no_of_nodes);
  cgraph_ivec_t v = *res;
  cgraph_ivec_fill(v, 0);

  for (CGRAPH_INTEGER ed = 0; ed < no_of_edges; ++ed) {
    if (loops || graph->from[ed] != graph->to[ed]) {
      if (mode & CGRAPH_IN) {
        v[ graph->to[ed] ] += 1;
      }
      if (mode & CGRAPH_OUT) {
        v[ graph->from[ed] ] += 1;
      }
    }
  }
  return 0;
}

int cgraph_degree_one(const cgraph_t *graph,
                      CGRAPH_INTEGER *res,
                      const CGRAPH_INTEGER node,
                      cgraph_neimode_t mode,
                      bool loops) {
  if (node < 0 || node > cgraph_vcount(graph) - 1) {
    CGRAPH_ERROR("cannot get degree");
  }
  if (mode != CGRAPH_OUT && mode != CGRAPH_IN &&
      mode != CGRAPH_ALL) {
    CGRAPH_ERROR("cannot get degree");
  }
  if (!cgraph_is_directed(graph)) {
    mode = CGRAPH_ALL;
  }
  CGRAPH_INTEGER d = 0;
  if (mode & CGRAPH_IN) {
    d += (graph->is[node + 1] - graph->is[node]);
  }
  if (mode & CGRAPH_OUT) {
    d += (graph->os[node + 1] - graph->os[node]);
  }
  if (!loops) {
    if (mode & CGRAPH_IN) {
      CGRAPH_INTEGER j = graph->is[node];
      CGRAPH_INTEGER j1 = graph->is[node + 1];
      for (CGRAPH_INTEGER i = j; i < j1; ++i) {
        CGRAPH_INTEGER idx = graph->ii[i];
        if (graph->from[ idx ] == graph->to[ idx ]) {
          --d;
        }
      }
    }
    if (mode & CGRAPH_OUT) {
      CGRAPH_INTEGER j = graph->os[node];
      CGRAPH_INTEGER j1 = graph->os[node + 1];
      for (CGRAPH_INTEGER i = j; i < j1; ++i) {
        CGRAPH_INTEGER idx = graph->oi[i];
        if (graph->from[ idx ] == graph->to[ idx ]) {
          --d;
        }
      }
    }
  }
  *res = d;
  return 0;
}

/**
 * \function Cgraph_edge
 * \brief Gives the head and tail vertices of an edge.
 *
 * \param graph The graph object.
 * \param eid The edge id.
 * \param from Pointer to an \type igraph_integer_t. The tail of the edge
 * will be placed here.
 * \param to Pointer to an \type igraph_integer_t. The head of the edge
 * will be placed here.
 * \return Error code. The current implementation always returns with
 * success.
 * for undirected graph, in the output from <= to
 * \sa \ref cgraph_get_eid() for the opposite operation;
 *     \ref CGRAPH_TO(), \ref CGRAPH_FROM() and \ref CGRAPH_OTHER() for
 *     a faster but non-error-checked version.
 *
 * Added in version 0.2.</para><para>
 *
 * Time complexity: O(1).
 */

int cgraph_edge(const cgraph_t *graph, CGRAPH_INTEGER eid,
               CGRAPH_INTEGER *from, CGRAPH_INTEGER *to) {
    if (cgraph_is_directed(graph)) {
        *from = (CGRAPH_INTEGER) (graph->from)[eid];
        *to   = (CGRAPH_INTEGER) (graph->to  )[eid];
    } else {
        *from = (CGRAPH_INTEGER) (graph->to  )[eid];
        *to   = (CGRAPH_INTEGER) (graph->from)[eid];
    }

    return 0;
}

/**
 * \function cgraph_get_eid
 * \brief Get the edge id from the end points of an edge.
 *
 * For undirected graphs \c pfrom and \c pto are exchangeable.
 *
 * \param graph The graph object.
 * \param eid Pointer to an integer, the edge id will be stored here.
 * \param pfrom The starting point of the edge.
 * \param pto The end point of the edge.
 * \param directed Logical constant, whether to search for directed
 *        edges in a directed graph. Ignored for undirected graphs.
 * \return Error code.
 * \sa \ref igraph_edge() for the opposite operation.
 *
 * Time complexity: O(log (d)), where d is smaller of the out-degree
 * of \c pfrom and in-degree of \c pto if \p directed is true. If \p directed
 * is false, then it is O(log(d)+log(d2)), where d is the same as before and
 * d2 is the minimum of the out-degree of \c pto and the in-degree of \c pfrom.
 *
 * \example examples/simple/igraph_get_eid.c
 *
 * Added in version 0.2.</para><para>
 */

#define BINSEARCH(start,end,value,iindex,edgelist,N,pos) \
  do {                                                   \
    while ((start) < (end)) {                            \
      CGRAPH_INTEGER mid = (start)+((end)-(start))/2;    \
      CGRAPH_INTEGER e = (iindex)[mid];                  \
      if ((edgelist)[e] < (value)) {                     \
        (start)=mid+1;                                   \
      } else {                                           \
        (end)=mid;                                       \
      }                                                  \
    }                                                    \
    if ((start)<(N)) {                                   \
      CGRAPH_INTEGER e=(iindex)[(start)];                \
      if ((edgelist)[e] == (value)) {                    \
        *(pos)=(CGRAPH_INTEGER) e;                       \
      }                                                  \
    }                                                    \
  } while(0)

#define FIND_DIRECTED_EDGE(graph,xfrom,xto,eid)          \
    do {                                                 \
        CGRAPH_INTEGER start = (graph->os)[xfrom];       \
        CGRAPH_INTEGER end = (graph->os)[xfrom+1];       \
        CGRAPH_INTEGER N = end;                          \
        CGRAPH_INTEGER start2 = (graph->is)[xto];        \
        CGRAPH_INTEGER end2 = (graph->is)[xto+1];        \
        CGRAPH_INTEGER N2 = end2;                        \
        if (end - start < end2 - start2) {                                \
            BINSEARCH(start,end,xto,graph->oi,graph->to,N,eid);           \
        } else {                                                          \
            BINSEARCH(start2,end2,xfrom,graph->ii,graph->from,N2,eid);    \
        }                                                                 \
    } while (0)

#define FIND_UNDIRECTED_EDGE(graph,from,to,eid)         \
    do {                                                \
        CGRAPH_INTEGER xfrom1= from > to ? from : to;   \
        CGRAPH_INTEGER xto1= from > to ? to : from;     \
        FIND_DIRECTED_EDGE(graph,xfrom1,xto1,eid);      \
    } while (0)

int cgraph_get_eid(const cgraph_t *graph, CGRAPH_INTEGER *eid,
                   CGRAPH_INTEGER pfrom, CGRAPH_INTEGER pto,
                   bool directed) {

    CGRAPH_INTEGER from = pfrom, to = pto;
    CGRAPH_INTEGER nov = cgraph_vcount(graph);

    if (from < 0 || to < 0 || from > nov - 1 || to > nov - 1) {
      CGRAPH_ERROR("cannot get edge id");
    }

    *eid = -1;
    if (cgraph_is_directed(graph)) {

      /* Directed graph */
      FIND_DIRECTED_EDGE(graph, from, to, eid);
      if (!directed && *eid < 0) {
        FIND_DIRECTED_EDGE(graph, to, from, eid);
      }
    } else {
      /* Undirected graph, they only have one mode */
      FIND_UNDIRECTED_EDGE(graph, from, to, eid);
    }

    if (*eid < 0) {
      CGRAPH_ERROR("Cannot get edge id, no such edge");
    }

    return 0;
}