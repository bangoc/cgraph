#include <stdio.h>

#include "cgraph_ivec.h"
#include "ut.h"

int main() {
  cgraph_ivec_t v = cgraph_ivec_create();
  int a[] = {
    0, 1,
    1, 3,
    1, 2,
  };
  int n = sizeof(a)/sizeof(int);
  cgraph_ivec_append(&v, a, n);

  if (cgraph_ivec_isininterval(v, 0, 3) != true) {
    UT_MSG_FAILED("Inbound test");
    return 1;
  }

  if (cgraph_ivec_isininterval(v, 0, 2) != false) {
    UT_MSG_FAILED("Upper bound test");
    return 1;
  }

  if (cgraph_ivec_isininterval(v, 1, 3) != false) {
    UT_MSG_FAILED("Lower bound test");
    return 1;
  }

  UT_MSG_OK("Vector is in interval test");
  return 0;
}