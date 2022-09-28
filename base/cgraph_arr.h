#ifndef BASE_CGRAPH_ARR_H_
#define BASE_CGRAPH_ARR_H_

/* (C) Nguyen Ba Ngoc 2022 */

/** @file
 * @brief Các triển khai riêng của arr cho số nguyên và số thực để
 * sử dụng trong cgraph
 */

#include "cgen/base/gtype.h"
#include "cgen/vec/arr.h"
#include "base/cgraph_datatype.h"

CGRAPH_INTEGER arr_imax(atype(CGRAPH_INTEGER) *a);

int arr_irange(atype(CGRAPH_INTEGER) *a,
               CGRAPH_INTEGER low, CGRAPH_INTEGER high);

atype(CGRAPH_INTEGER) *arr_ifrom_array(CGRAPH_INTEGER *a, CGRAPH_INTEGER n);

#define arr_ifill(v, value) \
  do { \
    for (long i = 0; i < arr_size(v); ++i) { \
      v[i] = value; \
    } \
  } while (0)

atype(CGRAPH_INTEGER) **arr_icreate_ref();
void gtype_free_iarr_ref(gtype v);

#define arr_iptr_at(vec, i) \
  (*((atype(CGRAPH_INTEGER) **)(gvec_elem(vec, i).v)))

int arr_iorder(atype(CGRAPH_INTEGER) *v,
               atype(CGRAPH_INTEGER) *v2,
               atype(CGRAPH_INTEGER) *res);

#define arr_swap(a, b) \
  do { \
    typeof(a) _tmp = (a); \
    (a) = (b); \
    (b) = _tmp; \
  } while (0)
#endif  // BASE_CGRAPH_ARR_H_