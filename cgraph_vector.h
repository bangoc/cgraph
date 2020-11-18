#ifndef CGRAPH_VECTOR_H_
#define CGRAPH_VECTOR_H_

#include "cgraph_basic_types.h"
#include "cvector.h"

typedef cvector_vector_type(CGRAPH_INTEGER) cgraph_ivec_t;  

CGRAPH_INTEGER cgraph_ivec_max(const cgraph_ivec_t v);

bool cgraph_ivec_isininterval(const cgraph_ivec_t v, CGRAPH_INTEGER low, CGRAPH_INTEGER high);

bool cgraph_ivec_grow(cgraph_ivec_t v, CGRAPH_INTEGER newcapacity);

bool cgraph_ivec_setsize(cgraph_ivec_t v, CGRAPH_INTEGER newsize);


#endif  // CGRAPH_VECTOR_H_