#include <stdio.h>

#include "cgraph_ivec.h"
#include "ut.h"

int main() {
  int a[] = {1, 2, 3, 4, 5};
  const int n = sizeof(a)/sizeof(int);
  bool any = false;
  if (n != 5) {
    any = true;
    UT_MSG_FAILED("Did you changed a?");
  }
  cgraph_ivec_t v = cgraph_ivec_from_array(a, n);
  if (cgraph_ivec_size(v) != n) {
    any = true;
    UT_MSG_FAILED("Wrong vector size");
  }
  for (int i = 0; i < n; ++i) {
    if (a[i] != v[i]) {
      any = true;
      UT_MSG_FAILED("Wrong value");
    }
  }
  if (any) {
    UT_MSG_FAILED("Test ivec append: failed");
    return 1;
  }
  return 0;
}