#ifndef CGRAPH_VISITOR_H_
#define CGRAPH_VISITOR_H_

#include "cgen/vec/arr.h"
#include "base/cgraph_datatype.h"
#include "base/cgraph_constants.h"

#ifdef __cplusplus
extern "C"{
#endif

int cgraph_bfs(const cgraph_t graph,
               CGRAPH_INTEGER root,
               cgraph_neimode_t mode,
               bool unreachable,
               arr_ptr(CGRAPH_INTEGER) restricted,
               arr_ptr(CGRAPH_INTEGER) *order,
               arr_ptr(CGRAPH_INTEGER) *rank,
               arr_ptr(CGRAPH_INTEGER) *father,
               arr_ptr(CGRAPH_INTEGER) *pred,
               arr_ptr(CGRAPH_INTEGER) *succ,
               arr_ptr(CGRAPH_INTEGER) *dist);

int cgraph_simple_bfs(const cgraph_t graph,
               CGRAPH_INTEGER root,
               cgraph_neimode_t mode,
               bool unreachable,
               arr_ptr(CGRAPH_INTEGER) *father,
               arr_ptr(CGRAPH_INTEGER) *dist);

int cgraph_dfs(const cgraph_t graph,
               CGRAPH_INTEGER root,
               cgraph_neimode_t mode,
               bool unreachable,
               arr_ptr(CGRAPH_INTEGER) *order,
               arr_ptr(CGRAPH_INTEGER) *order_out,
               arr_ptr(CGRAPH_INTEGER) *father,
               arr_ptr(CGRAPH_INTEGER) *dist);

#ifdef __cplusplus
}
#endif

#endif  // CGRAPH_VISITOR_H_
