#include <stdio.h>

#include "ut.h"

int arr_iequal(atype(CGRAPH_INTEGER) *v, CGRAPH_INTEGER *a, long n) {
  if (arr_size(v) != n) {
    return 0;
  }
  for (long i = 0; i < n; ++i) {
    if (v[i] != a[i]) {
      return 0;
    }
  }
  return 1;
}

void arr_iprint(atype(CGRAPH_INTEGER) *v) {
  printf("sz = %d, ", (int)arr_size(v));
  for (int i = 0; i < arr_size(v); ++i) {
    printf(" %d", v[i]);
  }
  printf("\n");
}

bool ut_is_connected(cgraph_t g, CGRAPH_INTEGER pfrom, CGRAPH_INTEGER pto,
        bool directed) {
  CGRAPH_INTEGER eid = -1;
  cgraph_get_eid(g, &eid, pfrom, pto, directed);
  return eid >= 0;
}