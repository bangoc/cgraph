#ifndef CGRAPH_VISITOR_H_
#define CGRAPH_VISITOR_H_

#include "cgen/vec/arr.h"
#include "base/cgraph_datatype.h"
#include "base/cgraph_constants.h"

#ifdef __cplusplus
extern "C"{
#endif

struct bfs *cgraph_bfs(const cgraph_t graph,
               CGRAPH_INTEGER root,
               cgraph_neimode_t mode,
               bool unreachable,
               atype(CGRAPH_INTEGER) *restricted);

struct dfs *cgraph_dfs(const cgraph_t graph,
               CGRAPH_INTEGER root,
               cgraph_neimode_t mode,
               bool unreachable);

#ifdef __cplusplus
}
#endif

#endif  // CGRAPH_VISITOR_H_
