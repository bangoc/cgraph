/* (C) Nguyen Ba Ngoc 2022 */

#include "base/cgraph_arr.h"

CGRAPH_INTEGER arr_imax(atype(CGRAPH_INTEGER) *a) {
  CGRAPH_INTEGER max;
  arr_assign_max(max, a);
  return max;
}

int arr_irange(atype(CGRAPH_INTEGER) *a,
               CGRAPH_INTEGER low, CGRAPH_INTEGER high) {
  arr_traverse(cur, a) {
    if (*cur < low || *cur > high) {
      return 0;
    }
  }
  return 1;
}

atype(CGRAPH_INTEGER) *arr_ifrom_array(CGRAPH_INTEGER *a, CGRAPH_INTEGER n) {
  arr_make(tmp, n, CGRAPH_INTEGER);
  for (long i = 0; i < n; ++i) {
    tmp[i] = a[i];
  }
  return tmp;
}

static atype(CGRAPH_INTEGER) *_v;
static atype(CGRAPH_INTEGER) *_v2;
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

int arr_iorder(atype(CGRAPH_INTEGER) *v,
               atype(CGRAPH_INTEGER) *v2,
               atype(CGRAPH_INTEGER) *res) {
  _v = v;
  _v2 = v2;
  long n = arr_size(v);
  for (long i = 0; i < n; ++i) {
    res[i] = i;
  }
  qsort(res, n, sizeof(CGRAPH_INTEGER), ref_cmp);
  return 0;
}

void gtype_arr_free(gtype v) {
  arr_free(v.v);
}