#include <stdio.h>
#include <stdlib.h>

#include "cgen/cgen.h"

void print_i(void *value) {
  printf(" %d", *((int*)value));
}

int main() {
  gsl_t q = gsl_create(NULL);
  printf("Enqueue\n");
  for (int value = 1; value <= 10; ++value) {
    printf(" %d", value);
    gsl_push_back(q, gtype_l(value));
  }
  printf("\n");
  printf("Queue size = %d\n", gsl_length(q));
  printf("Iterate\n");
  gsl_traverse(cur, q) {
    printf(" %ld", cur->l);
  }
  printf("\n");

  printf("Poll (get and remove front)\n");
  while (!gsl_is_empty(q)) {
    gtype *value = gsl_front(q);
    printf(" %ld", value->l);
    gsl_pop_front(q);
  }
  printf("\n");
  gsl_free(q);
  return 0;
}