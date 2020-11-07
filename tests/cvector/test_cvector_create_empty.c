#include <stdio.h>

#include "cvector.h"

int main() {
  const char *k_test_name = "Test cvector_create_empty";
  cvector_vector_type(int) v = cvector_create_empty();
  if (cvector_capacity(v) == 0 && cvector_size(v) == 0) {
    printf("%s: OK\n", k_test_name);
  } else {
    printf("%s: FAILED\n",k_test_name);
  }
  return 0;
}