#include <stdio.h>

#include "base/cvector.h"

void print_vector(cvector_vector_type(int) v) {
  printf("v(%lu): {", cvector_size(v));
  for (int i = 0; i < cvector_size(v); ++i) {
    if (i == 0) {
      printf("%d", v[i]);
    } else {
      printf(", %d", v[i]);
    }
  }
  printf("}\n");
}

int main() {
  cvector_vector_type(int) v = cvector_create_empty();
  cvector_push_back(v, 1);
  cvector_push_back(v, 2);
  cvector_push_back(v, 3);
  print_vector(v);
  cvector_pop_back(v);
  print_vector(v);
  cvector_push_back(v, 5);
  cvector_push_back(v, 6);
  cvector_push_back(v, 7);
  cvector_erase(v, 1);
  print_vector(v);
  cvector_free(v);
  return 0;
}