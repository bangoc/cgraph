#include <stdio.h>

#include "cgraph.h"

void print_vector(cgraph_ivec_t v) {
  printf("v(%lu): {", (unsigned long)cgraph_ivec_size(v));
  for (int i = 0; i < cgraph_ivec_size(v); ++i) {
    if (i == 0) {
      printf("%d", v[i]);
    } else {
      printf(", %d", v[i]);
    }
  }
  printf("}\n");
}

int main() {
  cgraph_ivec_t v = cgraph_ivec_create();
  int a[] = {
    0, 1,
    0, 2,
    1, 2,
    3, 5
  };
  int n = sizeof(a)/sizeof(int);
  for (int i = 0; i < n; ++i) {
    cgraph_ivec_push_back(&v, (CGRAPH_INTEGER)a[i]);
  }
  print_vector(v);
  cgraph_ivec_free(&v);
  return 0;
}