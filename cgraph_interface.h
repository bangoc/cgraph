#ifndef CGRAPH_INTERFACE_H_
#define CGRAPH_INTERFACE_H_

#include "cgraph_datatype.h"
#include "cgraph_constants.h"

#ifdef __cplusplus
extern "C"{
#endif

int cgraph_empty(cgraph_t *graph, CGRAPH_INTEGER n, bool directed);
int cgraph_add_vertices(cgraph_t *graph, CGRAPH_INTEGER nv);
void cgraph_destroy(cgraph_t *graph);
int cgraph_add_edges(cgraph_t *graph, cgraph_ivec_t const edges);
CGRAPH_INTEGER cgraph_vcount(const cgraph_t *graph);
CGRAPH_INTEGER cgraph_ecount(const cgraph_t *graph);
bool cgraph_is_directed(const cgraph_t *graph);
int cgraph_neighbors(const cgraph_t *graph,
                     cgraph_ivec_t* neis,
                     CGRAPH_INTEGER vid,
                     cgraph_neimode_t mode);
int cgraph_incident(const cgraph_t *graph,
                    cgraph_ivec_t* eids,
                    CGRAPH_INTEGER vid,
                    cgraph_neimode_t mode);

int cgraph_degree_all(const cgraph_t *graph,
                      cgraph_ivec_t *res,
                      cgraph_neimode_t mode,
                      bool loops);

int cgraph_degree_one(const cgraph_t *graph,
                      CGRAPH_INTEGER *res,
                      const CGRAPH_INTEGER node,
                      cgraph_neimode_t mode,
                      bool loops);
int cgraph_edge(const cgraph_t *graph, CGRAPH_INTEGER eid,
               CGRAPH_INTEGER *from, CGRAPH_INTEGER *to);
int cgraph_get_eid(const cgraph_t *graph, CGRAPH_INTEGER *eid,
                   CGRAPH_INTEGER pfrom, CGRAPH_INTEGER pto,
                   bool directed);

#ifdef __cplusplus
}
#endif

/**
 * \define CGRAPH_FROM
 * \brief The source vertex of an edge.
 *
 * Faster than \ref igraph_edge(), but no error checking is done: \p eid is assumed to be valid.
 *
 * \param graph The graph.
 * \param eid   The edge ID.
 * \return The source vertex of the edge.
 * \sa \ref igraph_edge() if error checking is desired.
 */
#define CGRAPH_FROM(graph,eid) ((CGRAPH_INTEGER)(graph->from[(long int)(eid)]))

/**
 * \define CGRAPH_TO
 * \brief The target vertex of an edge.
 *
 * Faster than \ref igraph_edge(), but no error checking is done: \p eid is assumed to be valid.
 *
 * \param graph The graph object.
 * \param eid   The edge ID.
 * \return The target vertex of the edge.
 * \sa \ref igraph_edge() if error checking is desired.
 */
#define CGRAPH_TO(graph,eid)   ((CGRAPH_INTEGER)(graph->to[(long int)(eid)]))

/**
 * \define CGRAPH_OTHER
 * \brief The other endpoint of an edge.
 *
 * Typically used with undirected edges when one endpoint of the edge is known,
 * and the other endpoint is needed. No error checking is done:
 * \p eid and \p vid are assumed to be valid.
 *
 * \param graph The graph object.
 * \param eid   The edge ID.
 * \param vid   The vertex ID of one endpoint of an edge.
 * \return The other endpoint of the edge.
 * \sa \ref IGRAPH_TO() and \ref IGRAPH_FROM() to get the source and target
 *     of directed edges.
 */
#define CGRAPH_OTHER(graph,eid,vid) \
    ((CGRAPH_INTEGER)(CGRAPH_TO(graph,(eid))==(vid) ? CGRAPH_FROM((graph),(eid)) : CGRAPH_TO((graph),(eid))))

#endif  // CGRAPH_INTERFACE_H_