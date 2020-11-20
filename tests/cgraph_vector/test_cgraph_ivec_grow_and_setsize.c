#include <stdio.h>

#include "cgraph_vector.h"
#include "ut.h"

int main() {
  cgraph_ivec_t v = cvector_create_empty();
  if (cvector_capacity(v) != 0 || cvector_size(v) != 0) {
    UT_MSG_FAILED("Test empty vector size and capacity");
    return 1;
  }

  cgraph_ivec_grow(&v, 10);
  if (cvector_capacity(v) != 10 || cvector_size(v) != 0) {
    UT_MSG_FAILED("Test grow 10");
    return 1;
  }

  cgraph_ivec_setsize(v, 10);
  if (cvector_capacity(v) != 10 || cvector_size(v) != 10) {
    UT_MSG_FAILED("Test setsize 10!");
    return 1;
  }

  UT_MSG_OK("Test grow and resize "
            "(incompleted, not cover address change in realloc)");
  return 0;
}