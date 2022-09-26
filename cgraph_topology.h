#ifndef CGRAPH_TOPOLOGY_H_
#define CGRAPH_TOPOLOGY_H_

#include "base/cgraph_datatype.h"

#ifdef __cplusplus
extern "C"{
#endif

bool cgraph_is_dag(const cgraph_t graph);
int cgraph_topological_sorting(const cgraph_t graph,
                               arr_ptr(CGRAPH_INTEGER) *res,
                               cgraph_neimode_t mode);

#ifdef __cplusplus
}
#endif

#endif  // CGRAPH_TOPOLOGY_H_