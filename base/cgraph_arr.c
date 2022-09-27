/* (C) Nguyen Ba Ngoc 2022 */

#include "base/cgraph_arr.h"

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

static arr_ptr(CGRAPH_INTEGER) _v;
static arr_ptr(CGRAPH_INTEGER) _v2;
static int ref_cmp(const void *o1, const void *o2) {
  CGRAPH_INTEGER i1 = *((CGRAPH_INTEGER*)o1),
                i2 = *((CGRAPH_INTEGER*)o2);
  if (_v[i1] < _v[i2]) {
    return -1;
  } else if (_v[i1] > _v[i2]) {
    return 1;
  }
  return _v2[i1] - _v2[i2];
}

int arr_iorder(arr_ptr(CGRAPH_INTEGER) v,
               arr_ptr(CGRAPH_INTEGER) v2,
               arr_ptr(CGRAPH_INTEGER) res) {
  _v = v;
  _v2 = v2;
  long n = arr_size(v);
  for (long i = 0; i < n; ++i) {
    res[i] = i;
  }
  qsort(res, n, sizeof(CGRAPH_INTEGER), ref_cmp);
  return 0;
}