#ifndef CGRAPH_ARR_H_
#define CGRAPH_ARR_H_

/* (C) Nguyen Ba Ngoc 2022 */

/** @file
 * @brief Các triển khai riêng của arr cho số nguyên và số thực để
 * sử dụng trong cgraph
 */

#include "cgen/base/gtype.h"
#include "cgen/vec/arr.h"
#include "cgraph_datatype.h"

CGRAPH_INTEGER arr_imax(arr_ptr(CGRAPH_INTEGER) a);

int arr_irange(arr_ptr(CGRAPH_INTEGER) a,
               CGRAPH_INTEGER low, CGRAPH_INTEGER high);

arr_ptr(CGRAPH_INTEGER) arr_ifrom_array(CGRAPH_INTEGER *a, CGRAPH_INTEGER n);

#define arr_ifill(v, value) \
  do { \
    for (long i = 0; i < arr_size(v); ++i) { \
      v[i] = value; \
    } \
  } while (0)

arr_ptr(CGRAPH_INTEGER) *arr_icreate_ref();
void gtype_free_iarr_ref(gtype v);

#define arr_iptr_at(vec, i) \
  (*((arr_ptr(CGRAPH_INTEGER)*)(gvec_elem(vec, i).v)))

void arr_iminmax(arr_ptr(CGRAPH_INTEGER) a, CGRAPH_INTEGER *min, CGRAPH_INTEGER *max);

int arr_iorder(arr_ptr(CGRAPH_INTEGER) v,
               arr_ptr(CGRAPH_INTEGER) v2,
               arr_ptr(CGRAPH_INTEGER) res);

#endif  // CGRAPH_ARR_H_