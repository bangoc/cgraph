#ifndef CGRAPH_PATHS_H_
#define CGRAPH_PATHS_H_

#include "cgen/all.h"
#include "base/cgraph_datatype.h"
#include "base/cgraph_constants.h"

#ifdef __cplusplus
extern "C"{
#endif

struct path *cgraph_get_shortest_path(const cgraph_t graph,
        const CGRAPH_INTEGER from,
        const CGRAPH_INTEGER to,
        const cgraph_neimode_t mode);

struct paths *cgraph_get_shortest_paths_dijkstra(const cgraph_t graph,
        CGRAPH_INTEGER from,
        atype(CGRAPH_INTEGER) *to,
        atype(CGRAPH_REAL) *weights,
        cgraph_neimode_t mode);

struct paths *cgraph_get_shortest_paths(const cgraph_t graph,
        CGRAPH_INTEGER from,
        atype(CGRAPH_INTEGER) *to,
        cgraph_neimode_t mode);

struct path *cgraph_get_shortest_path_dijkstra(const cgraph_t graph,
        CGRAPH_INTEGER from,
        CGRAPH_INTEGER to,
        atype(CGRAPH_REAL) *weights,
        cgraph_neimode_t mode);

#ifdef __cplusplus
}
#endif

#endif  // CGRAPH_PATHS_H_
