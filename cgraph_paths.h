#ifndef CGRAPH_PATHS_H_
#define CGRAPH_PATHS_H_

#include "cgen/all.h"
#include "cgraph_datatype.h"
#include "cgraph_constants.h"
#include "cgraph_ivec.h"

#ifdef __cplusplus
extern "C"{
#endif

int cgraph_get_shortest_path(const cgraph_t graph,
        cgraph_ivec_t *vertices,
        cgraph_ivec_t *edges,
        const CGRAPH_INTEGER from,
        const CGRAPH_INTEGER to,
        const cgraph_neimode_t mode);

int cgraph_get_shortest_paths_dijkstra(const cgraph_t graph,
        gvec_t vertices,
        gvec_t edges,
        CGRAPH_INTEGER from,
        cgraph_ivec_t to,
        arr_ptr(CGRAPH_REAL) weights,
        cgraph_neimode_t mode,
        cgraph_ivec_t *predecessors,
        cgraph_ivec_t *inbound_edges);

int cgraph_get_shortest_paths(const cgraph_t graph,
        gvec_t vertices,
        gvec_t edges,
        CGRAPH_INTEGER from,
        cgraph_ivec_t to,
        cgraph_neimode_t mode,
        cgraph_ivec_t *predecessors,
        cgraph_ivec_t *inbound_edges);

int cgraph_get_shortest_path_dijkstra(const cgraph_t graph,
        cgraph_ivec_t *vertices,
        cgraph_ivec_t *edges,
        CGRAPH_INTEGER from,
        CGRAPH_INTEGER to,
        arr_ptr(CGRAPH_REAL) weights,
        cgraph_neimode_t mode);

#ifdef __cplusplus
}
#endif

#endif  // CGRAPH_PATHS_H_
