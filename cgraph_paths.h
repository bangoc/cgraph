#ifndef CGRAPH_PATHS_H_
#define CGRAPH_PATHS_H_

#include "cgraph_datatype.h"
#include "cgraph_constants.h"
#include "cgraph_ivec.h"
#include "cgraph_rvec.h"

#ifdef __cplusplus
extern "C"{
#endif

int cgraph_get_shortest_path(const cgraph_t *graph,
        cgraph_ivec_t *vertices,
        cgraph_ivec_t *edges,
        CGRAPH_INTEGER from,
        CGRAPH_INTEGER to,
        cgraph_neimode_t mode);

int cgraph_get_shortest_path_dijkstra(const cgraph_t *graph,
        cgraph_ivec_t *vertices,
        cgraph_ivec_t *edges,
        CGRAPH_INTEGER from,
        CGRAPH_INTEGER to,
        cgraph_rvec_t weights,
        cgraph_neimode_t mode);

#ifdef __cplusplus
}
#endif

#endif  // CGRAPH_PATHS_H_