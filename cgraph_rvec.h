#ifndef CGRAPH_RVEC_H_
#define CGRAPH_RVEC_H_

#include <assert.h>
#include <stddef.h>

#include "cgraph_basic_types.h"

typedef CGRAPH_REAL *cgraph_rvec_t;

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

cgraph_rvec_t cgraph_rvec_create();

/* Pass vector pointer by value */
int cgraph_rvec_setsize(cgraph_rvec_t const v,
                        CGRAPH_INTEGER newsize);
CGRAPH_INTEGER cgraph_rvec_capacity(cgraph_rvec_t const v);
CGRAPH_INTEGER cgraph_rvec_size(cgraph_rvec_t const v);
int cgraph_rvec_print(cgraph_rvec_t const v);

/* Pass vector pointer by reference */
int cgraph_rvec_grow(cgraph_rvec_t *v,
                     CGRAPH_INTEGER newcapacity);
int cgraph_rvec_init(cgraph_rvec_t *v,
                     CGRAPH_INTEGER size);

int cgraph_rvec_push_back(cgraph_rvec_t *v,
                          CGRAPH_REAL value);
int cgraph_rvec_free(cgraph_rvec_t *v);

#ifdef __cplusplus
}
#endif

#endif  // CGRAPH_RVEC_H_