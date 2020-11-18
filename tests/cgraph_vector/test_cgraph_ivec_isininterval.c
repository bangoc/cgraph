#include <stdio.h>

#include "cgraph_vector.h"

int main() {
  cgraph_ivec_t v = cvector_create_empty();
  int a[] = {
    0, 1,
    1, 3,
    1, 2,
  };
  int n = sizeof(a)/sizeof(int);
  for (int i = 0; i < n; ++i) {
    cvector_push_back(v, (CGRAPH_INTEGER)a[i]);
  }

  if (cgraph_ivec_isininterval(v, 0, 3) != true) {
    printf("%s: FAILED\n", "Inbound test");
    return 1;
  }

  if (cgraph_ivec_isininterval(v, 0, 2) != false) {
    printf("%s: FAILED\n", "Upper bound test");
    return 1;
  }

  if (cgraph_ivec_isininterval(v, 1, 3) != false) {
    printf("%s: FAILED\n", "Lower bound test");
    return 1;
  }

  printf("%s: OK\n", "Vector is in interval test");
  return 0;
}