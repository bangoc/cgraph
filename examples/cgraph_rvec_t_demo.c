#include <stdio.h>

#include "cgraph.h"

int main() {
  cgraph_rvec_t v = cgraph_rvec_create();
  double a[] = {
    0.01, 1.22,
    0.5, 2.333,
    1.111, 2.222,
    3.001, 0.00005,
    1.00101
  };
  int n = sizeof(a)/sizeof(double);
  for (int i = 0; i < n; ++i) {
    cgraph_rvec_push_back(&v, (CGRAPH_REAL)a[i]);
  }
  cgraph_rvec_print(v);
  cgraph_rvec_free(&v);
  return 0;
}