#ifndef CGRAPH_VISITOR_H_
#define CGRAPH_VISITOR_H_

#include "cgraph_datatype.h"
#include "cgraph_constants.h"

int cgraph_bfs(const cgraph_t *graph,
               CGRAPH_INTEGER root,
               cgraph_neimode_t mode, 
               bool unreachable,
               const cgraph_ivec_t *restricted,
               cgraph_ivec_t *order, 
               cgraph_ivec_t *rank,
               cgraph_ivec_t *father,
               cgraph_ivec_t *pred, 
               cgraph_ivec_t *succ,
               cgraph_ivec_t *dist);

#endif  // CGRAPH_VISITOR_H_