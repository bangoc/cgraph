#include <stdio.h>

#include "base/cvector.h"
#include "ut.h"

int main() {
  const char *k_test_name = "Test cvector_create_empty";
  cvector_vector_type(int) v = cvector_create_empty();
  if (cvector_capacity(v) == 0 && cvector_size(v) == 0) {
    UT_MSG_OK(k_test_name);
    return 0;
  }
  UT_MSG_FAILED(k_test_name);
  return 1;
}