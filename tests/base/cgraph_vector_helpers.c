#include "ut.h"

cgraph_ivec_t cgraph_ivec_fromarray(CGRAPH_INTEGER *a, CGRAPH_INTEGER n) {
  cgraph_ivec_t v = cgraph_ivec_create();
  for (CGRAPH_INTEGER i = 0; i < n; ++i) {
    cgraph_ivec_push_back(&v, a[i]);
  }
  return v;
}

bool cgraph_ivec_equal(cgraph_ivec_t v, CGRAPH_INTEGER *a, CGRAPH_INTEGER n) {
  CGRAPH_INTEGER sz = cgraph_ivec_size(v);
  if (sz != n) {
    return false;
  }
  for (int i = 0; i < sz; ++i) {
    if (v[i] != a[i]) {
      return false;
    }
  }
  return true;
}