#ifndef CGRAPH_PATHS_H_
#define CGRAPH_PATHS_H_

#include "cgraph_datatype.h"
#include "cgraph_constants.h"

int cgraph_get_shortest_path_dijkstra(const cgraph_t *graph,
        cgraph_ivec_t *vertices,
        cgraph_ivec_t *edges,
        CGRAPH_INTEGER from,
        CGRAPH_INTEGER to,
        const double *weights,
        cgraph_neimode_t mode);

#endif  // CGRAPH_PATHS_H_