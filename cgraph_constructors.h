#ifndef CGRAPH_CONSTRUCTORS_H_
#define CGRAPH_CONSTRUCTORS_H_

#include "cgraph_datatype.h"
#include "cgraph_interface.h" // cgraph_is_directed()
#include <stdio.h>  // for printf
#include <string.h> // for memset

typedef struct att {
    int from;
    int to;
    int index;
} attribute;


int cgraph_create(cgraph_t *graph, cgraph_ivec_t edges, 
                    CGRAPH_INTEGER n, bool directed);


#endif  // CGRAPH_CONSTRUCTORS_H_