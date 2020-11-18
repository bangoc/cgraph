#include "cgraph_vector.h"

CGRAPH_INTEGER cgraph_ivec_max(const cgraph_ivec_t v) {
  CGRAPH_INTEGER max = v[0];
  for (CGRAPH_INTEGER i = 1; i < cvector_size(v); ++i) {
    if (v[i] > max) {
      max = v[i];
    }
  }
  return max;
}

bool cgraph_ivec_isininterval(const cgraph_ivec_t v, CGRAPH_INTEGER low, CGRAPH_INTEGER high) {
  for (CGRAPH_INTEGER i = 0; i < cvector_size(v); ++i) {
    if (v[i] < low || v[i] > high) {
      return false;
    }
  }
  return true;
}