#ifndef CGRAPH_VISITOR_H_
#define CGRAPH_VISITOR_H_

#include "cgraph_datatype.h"
#include "cgraph_constants.h"

#ifdef __cplusplus
extern "C"{
#endif 

int cgraph_bfs(const cgraph_t *graph,
               CGRAPH_INTEGER root,
               cgraph_neimode_t mode, 
               bool unreachable,
               cgraph_ivec_t const restricted,
               cgraph_ivec_t *order, 
               cgraph_ivec_t *rank,
               cgraph_ivec_t *father,
               cgraph_ivec_t *pred, 
               cgraph_ivec_t *succ,
               cgraph_ivec_t *dist);

int cgraph_simple_bfs(const cgraph_t *graph,
               CGRAPH_INTEGER root,
               cgraph_neimode_t mode, 
               bool unreachable,
               cgraph_ivec_t *father,
               cgraph_ivec_t *dist);

int cgraph_dfs(const cgraph_t *graph,
               CGRAPH_INTEGER root,
               cgraph_neimode_t mode, 
               bool unreachable,
               cgraph_ivec_t *order,
               cgraph_ivec_t *order_out, 
               cgraph_ivec_t *father,
               cgraph_ivec_t *dist);

#ifdef __cplusplus
}
#endif 

#endif  // CGRAPH_VISITOR_H_