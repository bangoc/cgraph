#ifndef CGRAPH_FOREIGN_H_
#define CGRAPH_FOREIGN_H_

#include <stdio.h>

#ifdef __cplusplus
extern "C"{
#endif

int cgraph_write_graph_dot(const cgraph_t graph, FILE *outstream);


#ifdef __cplusplus
}
#endif

#endif  // CGRAPH_FOREIGN_H_