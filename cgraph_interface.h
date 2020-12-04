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

#ifdef __cplusplus
}
#endif 

#endif  // CGRAPH_INTERFACE_H_