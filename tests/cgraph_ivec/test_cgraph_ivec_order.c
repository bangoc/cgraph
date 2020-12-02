#include <stdio.h>

#include "cgraph_error.h"
#include "cgraph_ivec.h"
#include "ut.h"

int main() {
  cgraph_ivec_t v = cgraph_ivec_fromarray((CGRAPH_INTEGER []){1, 0, 2, 2, 3, 1}, 6);
  cgraph_ivec_t v2 = cgraph_ivec_fromarray((CGRAPH_INTEGER []){3, 2, 1, 3, 2, 0}, 6);
  cgraph_ivec_t res = cgraph_ivec_create();
  CGRAPH_CHECK(cgraph_ivec_init(&res, 6));
  cgraph_ivec_order(v, v2, res);
  if (cgraph_ivec_size(res) != 6) {
    UT_MSG_FAILED("Test ivec init size");
    return 1;
  }
  if (!cgraph_ivec_equal(res, (CGRAPH_INTEGER[]){1, 5, 0, 2, 3, 4}, 6)) {
    UT_MSG_FAILED("Test index");
    return 1;
  }
  UT_MSG_OK("cgraph_ivec_order test");
  return 0;
}