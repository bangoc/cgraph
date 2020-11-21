#ifndef CGRAPH_VECTOR_H_
#define CGRAPH_VECTOR_H_

#include "cgraph_basic_types.h"
#include "base/cvector.h"

typedef cvector_vector_type(CGRAPH_INTEGER) cgraph_ivec_t;  

/* Pass vector pointer by value */
CGRAPH_INTEGER cgraph_ivec_max(const cgraph_ivec_t v);
bool cgraph_ivec_isininterval(const cgraph_ivec_t v,
                              CGRAPH_INTEGER low,
                              CGRAPH_INTEGER high);
int cgraph_ivec_order(const cgraph_ivec_t v, 
                      const cgraph_ivec_t v2,
                      cgraph_ivec_t res);
int cgraph_ivec_null(cgraph_ivec_t v);
int cgraph_ivec_setsize(cgraph_ivec_t v, 
                        CGRAPH_INTEGER newsize);
CGRAPH_INTEGER cgraph_ivec_capacity(const cgraph_ivec_t v);
CGRAPH_INTEGER cgraph_ivec_size(const cgraph_ivec_t v);
cgraph_ivec_t cgraph_ivec_create();

/* Pass vector pointer by reference */
int cgraph_ivec_grow(cgraph_ivec_t *v, 
                     CGRAPH_INTEGER newcapacity);
int cgraph_ivec_init(cgraph_ivec_t *v, 
                     CGRAPH_INTEGER size);

int cgraph_ivec_push_back(cgraph_ivec_t *v,
                          CGRAPH_INTEGER value);
int cgraph_ivec_free(cgraph_ivec_t *v);

#endif  // CGRAPH_VECTOR_H_