#ifndef CGRAPH_VECTOR_H_
#define CGRAPH_VECTOR_H_

#include "cgraph_basic_types.h"
#include "base/cvector.h"

typedef cvector_vector_type(CGRAPH_INTEGER) cgraph_ivec_t;  

CGRAPH_INTEGER cgraph_ivec_max(const cgraph_ivec_t v);

bool cgraph_ivec_isininterval(const cgraph_ivec_t v, CGRAPH_INTEGER low, CGRAPH_INTEGER high);

int cgraph_ivec_order(const cgraph_ivec_t v, const cgraph_ivec_t v2,
                        cgraph_ivec_t res);

int cgraph_ivec_null(cgraph_ivec_t v);

#define cgraph_ivec_grow(v, newcapacity) cgraph_ivec_grow_internal((v), newcapacity)
#define cgraph_ivec_grow_internal(v, newcapacity) \
  do { \
    CGRAPH_INTEGER capacity = (CGRAPH_INTEGER) cvector_capacity(v); \
    if (capacity < newcapacity) { \
      cvector_grow(v, newcapacity); \
    } \
  } while(0)

#define cgraph_ivec_setsize(v, newsize) cgraph_ivec_setsize_internal((v), newsize)
#define cgraph_ivec_setsize_internal(v, newsize) \
  do { \
    CGRAPH_INTEGER capacity = (CGRAPH_INTEGER) cvector_capacity(v); \
    if (newsize <= capacity) { \
      cvector_set_size(v, newsize); \
    } \
  } while (0)

#define cgraph_ivec_init(v, size) cgraph_ivec_init_internal((v), size)
#define cgraph_ivec_init_internal(v, size) \
  do { \
    cgraph_ivec_grow(v, size); \
    cgraph_ivec_setsize(v, size); \
  } while (0)

#define cgraph_ivec_capacity(v) cvector_capacity((v))
#define cgraph_ivec_size(v) cvector_size((v))

#define cgraph_ivec_create() cvector_create_empty()

#define cgraph_ivec_push_back(v, value) cvector_push_back((v), ((CGRAPH_INTEGER)value))

#define cgraph_ivec_free(v) cvector_free((v))

#endif  // CGRAPH_VECTOR_H_