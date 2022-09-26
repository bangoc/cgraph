/* (C) Nguyen Ba Ngoc 2022 */

#include "cgraph_arr.h"

CGRAPH_INTEGER arr_imax(arr_ptr(CGRAPH_INTEGER) a) {
  CGRAPH_INTEGER max = a[0];
  for (long i = 1; i < arr_size(a); ++i) {
    if (a[i] > max) {
      max = a[i];
    }
  }
  return max;
}

int arr_irange(arr_ptr(CGRAPH_INTEGER) a,
               CGRAPH_INTEGER low, CGRAPH_INTEGER high) {
  for (long i = 0; i < arr_size(a); ++i) {
    if (a[i] < low || a[i] > high) {
      return 0;
    }
  }
  return 1;
}

arr_ptr(CGRAPH_INTEGER) arr_ifrom_array(CGRAPH_INTEGER *a, CGRAPH_INTEGER n) {
  arr_make(tmp, n, CGRAPH_INTEGER);
  for (long i = 0; i < n; ++i) {
    tmp[i] = a[i];
  }
  return tmp;
}

arr_ptr(CGRAPH_INTEGER) *arr_icreate_ref() {
  arr_ptr(CGRAPH_INTEGER) *vref = malloc(sizeof(arr_ptr(CGRAPH_INTEGER)));
  *vref = arr_create(0, CGRAPH_INTEGER);
  return vref;
}

void gtype_free_iarr_ref(gtype v) {
  arr_ptr(CGRAPH_INTEGER) *vref = (arr_ptr(CGRAPH_INTEGER) *)(v.v);
  arr_free(*vref);
  free(vref);
}