#ifndef CGRAPH_PATHS_H_
#define CGRAPH_PATHS_H_

#include "cgen/all.h"
#include "base/cgraph_datatype.h"
#include "base/cgraph_constants.h"

#ifdef __cplusplus
extern "C"{
#endif

int cgraph_get_shortest_path(const cgraph_t graph,
        arr_ptr(CGRAPH_INTEGER) *vertices,
        arr_ptr(CGRAPH_INTEGER) *edges,
        const CGRAPH_INTEGER from,
        const CGRAPH_INTEGER to,
        const cgraph_neimode_t mode);

int cgraph_get_shortest_paths_dijkstra(const cgraph_t graph,
        gvec_t vertices,
        gvec_t edges,
        CGRAPH_INTEGER from,
        arr_ptr(CGRAPH_INTEGER) to,
        arr_ptr(CGRAPH_REAL) weights,
        cgraph_neimode_t mode,
        arr_ptr(CGRAPH_INTEGER) *predecessors,
        arr_ptr(CGRAPH_INTEGER) *inbound_edges);

int cgraph_get_shortest_paths(const cgraph_t graph,
        gvec_t vertices,
        gvec_t edges,
        CGRAPH_INTEGER from,
        arr_ptr(CGRAPH_INTEGER) to,
        cgraph_neimode_t mode,
        arr_ptr(CGRAPH_INTEGER) *predecessors,
        arr_ptr(CGRAPH_INTEGER) *inbound_edges);

int cgraph_get_shortest_path_dijkstra(const cgraph_t graph,
        arr_ptr(CGRAPH_INTEGER) *vertices,
        arr_ptr(CGRAPH_INTEGER) *edges,
        CGRAPH_INTEGER from,
        CGRAPH_INTEGER to,
        arr_ptr(CGRAPH_REAL) weights,
        cgraph_neimode_t mode);

#ifdef __cplusplus
}
#endif

#endif  // CGRAPH_PATHS_H_
