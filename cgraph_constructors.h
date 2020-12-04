#ifndef CGRAPH_CONSTRUCTORS_H_
#define CGRAPH_CONSTRUCTORS_H_

#include "cgraph_datatype.h"

#ifdef __cplusplus
extern "C"{
#endif 

int cgraph_create(cgraph_t *graph, 
                  cgraph_ivec_t const edges, 
                  CGRAPH_INTEGER n, 
                  bool directed);

#ifdef __cplusplus
}
#endif

#endif  // CGRAPH_CONSTRUCTORS_H_