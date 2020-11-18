#ifndef CGRAPH_INTERFACE_H_
#define CGRAPH_INTERFACE_H_

#include "cgraph_datatype.h"
#include "cgraph_constants.h"

int cgraph_empty(cgraph_t *graph, CGRAPH_INTEGER n, bool directed);
int cgraph_add_vertices(cgraph_t *graph, CGRAPH_INTEGER nv);
void cgraph_destroy(cgraph_t *graph);
int cgraph_add_edges(cgraph_t *graph, const cgraph_ivec_t edges);
CGRAPH_INTEGER cgraph_vcount(const cgraph_t *graph);
CGRAPH_INTEGER cgraph_ecount(const cgraph_t *graph);
bool cgraph_is_directed(const cgraph_t *graph);
int cgraph_neighbors(const cgraph_t *graph, 
                     cgraph_ivec_t *neis, 
                     CGRAPH_INTEGER vid,
                     cgraph_neimode_t mode);
int cgraph_adjacent(const cgraph_t *graph, 
                    cgraph_ivec_t *eids, 
                    CGRAPH_INTEGER vid,
                    cgraph_neimode_t mode); 


#endif  // CGRAPH_INTERFACE_H_