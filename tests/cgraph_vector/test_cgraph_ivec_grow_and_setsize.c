#include <stdio.h>

#include "cgraph_vector.h"

int main() {
  cgraph_ivec_t v = cvector_create_empty();
  if (cvector_capacity(v) != 0 || cvector_size(v) != 0) {
    printf("%s: FAILED\n", "Test empty vector size and capacity");
    return 1;
  }

  cgraph_ivec_grow(v, 10);
  if (cvector_capacity(v) != 10 || cvector_size(v) != 0) {
    printf("%s: FAILED\n", "Test grow 10");
    return 1;
  }

  cgraph_ivec_setsize(v, 10);
  if (cvector_capacity(v) != 10 || cvector_size(v) != 10) {
    printf("%s: FAILED\n", "Test setsize 10!");
    return 1;
  }

  return 0;
}