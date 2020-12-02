#include <stdio.h>

#include "cgraph_ivec.h"
#include "ut.h"

int main() {
  const char *k_test_name = "Test cgraph_ivec_max";
  cgraph_ivec_t v = cgraph_ivec_create();
  int a[] = {
    0, 1,
    1, 3,
    1, 2,
    3, 5,
    2, 3
  };
  int n = sizeof(a)/sizeof(int);
  for (int i = 0; i < n; ++i) {
    cgraph_ivec_push_back(&v, (CGRAPH_INTEGER)a[i]);
  }
  if (cgraph_ivec_max(v) == 5) {
    UT_MSG_OK(k_test_name);
    return 0;
  }
  UT_MSG_FAILED(k_test_name);
  return 1;
}