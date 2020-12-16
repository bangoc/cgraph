#ifndef CGRAPH_IVEC_H_
#define CGRAPH_IVEC_H_

#include <assert.h>
#include <stddef.h>

#include "cgraph_basic_types.h"

typedef CGRAPH_INTEGER *cgraph_ivec_t;

// cgraph_ivec_t is a CGRAPH_INTEGER * pointer, but
//
// For a vector, we will allocate memory with a special layout,
// what is different from an usual array.
// So, it's desireable to use a distinctive name, and cgraph_ivec_t is chosen.
//
// If you ask why should we go through all of these pains?
//   Well, random access to an arbitrary element by index in the primitive form
//              e.g, v[i]
//   looked so attractive, right?

#ifdef __cplusplus
extern "C"{
#endif

cgraph_ivec_t cgraph_ivec_create();

/* Pass vector pointer by value */
CGRAPH_INTEGER cgraph_ivec_max(cgraph_ivec_t const v);
int cgraph_ivec_minmax(cgraph_ivec_t const v, CGRAPH_INTEGER *min, CGRAPH_INTEGER *max);
bool cgraph_ivec_isininterval(cgraph_ivec_t const v,
                              CGRAPH_INTEGER low,
                              CGRAPH_INTEGER high);
int cgraph_ivec_order(cgraph_ivec_t const v,
                      cgraph_ivec_t const v2,
                      cgraph_ivec_t const res);
int cgraph_ivec_null(cgraph_ivec_t const v);
int cgraph_ivec_setsize(cgraph_ivec_t const v,
                        CGRAPH_INTEGER newsize);
CGRAPH_INTEGER cgraph_ivec_capacity(cgraph_ivec_t const v);
CGRAPH_INTEGER cgraph_ivec_size(cgraph_ivec_t const v);
int cgraph_ivec_fill(cgraph_ivec_t const v, CGRAPH_INTEGER data);
int cgraph_ivec_print(cgraph_ivec_t const v);

/* Pass vector pointer by reference */
int cgraph_ivec_grow(cgraph_ivec_t *v,
                     CGRAPH_INTEGER newcapacity);
int cgraph_ivec_init(cgraph_ivec_t *v,
                     CGRAPH_INTEGER size);

int cgraph_ivec_push_back(cgraph_ivec_t *v,
                          CGRAPH_INTEGER value);
int cgraph_ivec_free(cgraph_ivec_t *v);

#ifdef __cplusplus
}
#endif

#endif  // CGRAPH_IVEC_H_