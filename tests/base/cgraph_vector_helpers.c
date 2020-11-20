#include "ut.h"

cgraph_ivec_t cgraph_ivec_fromarray(CGRAPH_INTEGER *a, CGRAPH_INTEGER n) {
  cgraph_ivec_t v = cvector_create_empty();
  for (CGRAPH_INTEGER i = 0; i < n; ++i) {
    cvector_push_back(v, a[i]);
  }
  return v;
}

bool cgraph_ivec_equal(cgraph_ivec_t v, CGRAPH_INTEGER *a) {
  CGRAPH_INTEGER n = cvector_size(v);
  for (int i = 0; i < n; ++i) {
    if (v[i] != a[i]) {
      return false;
    }
  }
  return true;
}