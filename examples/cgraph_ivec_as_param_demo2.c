#include <stdio.h>

#include "cgraph.h"

int foo(cgraph_ivec_t *v, long n) {
  cgraph_ivec_t old = *v;
  for (int i = 0; i < n; ++i) {
    cgraph_ivec_push_back(v, i);
    if (old != *v) {
      return 1;
    }
  }
  return 0;
}

int main(int argc, char *argv[]) {
  cgraph_ivec_t v = cgraph_ivec_create();
  long n = 100000;
  if (argc > 1) {
    sscanf(argv[1], "%ld", &n);
  }
  int ret = foo(&v, n);
  if (ret != 0) {
    printf("Reallocated in a different location, "
           "but it's still safe to use v further.\n");
    printf("%lu\n", (unsigned long)cgraph_ivec_size(v));
    return 0;
  }
  printf("Address is stay the same in this case, size = %lu\n", 
              (unsigned long)cgraph_ivec_size(v));
  return 0;
}