#ifndef CGRAPH_TYPES_INTERNAL_H_
#define CGRAPH_TYPES_INTERNAL_H_

#include "cgraph_basic_types.h"
#include "cgraph_ivec.h"
#include "cgraph_rvec.h"

/* -------------------------------------------------- */
/* Two-way indexed heap                               */
/* -------------------------------------------------- */

/* This is a smart indexed heap. In addition to the "normal" indexed heap
   it allows to access every element through its index in O(1) time.
   In other words, for this heap the _modify operation is O(1), the
   normal heap does this in O(n) time.... */

typedef struct cgraph_2wheap_t {
    CGRAPH_INTEGER size;
    cgraph_rvec_t data;
    cgraph_ivec_t index;
    cgraph_ivec_t index2;
} cgraph_2wheap_t;

#ifdef __cplusplus
extern "C"{
#endif

int cgraph_2wheap_init(cgraph_2wheap_t *h, CGRAPH_INTEGER size);
void cgraph_2wheap_free(cgraph_2wheap_t *h);
int cgraph_2wheap_clear(cgraph_2wheap_t *h);
int cgraph_2wheap_push_with_index(cgraph_2wheap_t *h,
                                  CGRAPH_INTEGER idx, CGRAPH_REAL elem);
bool cgraph_2wheap_empty(const cgraph_2wheap_t *h);
CGRAPH_INTEGER cgraph_2wheap_size(const cgraph_2wheap_t *h);
CGRAPH_INTEGER cgraph_2wheap_max_size(const cgraph_2wheap_t *h);
CGRAPH_REAL cgraph_2wheap_max(const cgraph_2wheap_t *h);
CGRAPH_INTEGER cgraph_2wheap_max_index(const cgraph_2wheap_t *h);
CGRAPH_REAL cgraph_2wheap_deactivate_max(cgraph_2wheap_t *h);
bool cgraph_2wheap_has_elem(const cgraph_2wheap_t *h, CGRAPH_INTEGER idx);
bool cgraph_2wheap_has_active(const cgraph_2wheap_t *h, CGRAPH_INTEGER idx);
CGRAPH_REAL cgraph_2wheap_get(const cgraph_2wheap_t *h, CGRAPH_INTEGER idx);
CGRAPH_REAL cgraph_2wheap_delete_max(cgraph_2wheap_t *h);
CGRAPH_REAL cgraph_2wheap_delete_max_index(cgraph_2wheap_t *h, CGRAPH_INTEGER *idx);
int cgraph_2wheap_modify(cgraph_2wheap_t *h, CGRAPH_INTEGER idx, CGRAPH_REAL elem);
int cgraph_2wheap_check(cgraph_2wheap_t *h);

#ifdef __cplusplus
}
#endif

#endif  // CGRAPH_TYPES_INTERNAL_H_