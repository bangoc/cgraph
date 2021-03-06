#include <stdio.h>

#include "ut.h"

bool cgraph_ivec_equal(cgraph_ivec_t const v, CGRAPH_INTEGER *a, CGRAPH_INTEGER n) {
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

void print_ivec(cgraph_ivec_t const v) {
  printf("sz = %d, ", (int)cgraph_ivec_size(v));
  for (int i = 0; i < cgraph_ivec_size(v); ++i) {
    printf(" %d", v[i]);
  }
  printf("\n");
}