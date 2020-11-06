#ifndef CGRAPH_CONSTRUCTORS_H_
#define CGRAPH_CONSTRUCTORS_H_

#include "cgraph_datatype.h"

int cgraph_create(cgraph_t *graph, cgraph_ivec_t edges, CGRAPH_INTEGER n,
                  bool directed);

#endif  // CGRAPH_CONSTRUCTORS_H_