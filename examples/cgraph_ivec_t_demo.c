#include <stdio.h>

#include "cgraph.h"

void print_vector(cgraph_ivec_t v) {
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
  cgraph_ivec_t v = cvector_create_empty();
  int a[] = {
    0, 1,
    0, 2,
    1, 2,
    3, 5
  };
  int n = sizeof(a)/sizeof(int);
  for (int i = 0; i < n; ++i) {
    cvector_push_back(v, (CGRAPH_INTEGER)a[i]);
  }
  print_vector(v);
  cvector_free(v);
}