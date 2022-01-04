#include <stdio.h>
#include <stdlib.h>
#include "cgen/cgen.h"

int main() {
  p1w_t q = p1w_create(PRIORITY_MAX, gtype_cmp_d);
  double data[] = {0.111, 2.22222, 3.3333, -5.5555,
                   9.9999, -1.1111, 10.10101, -3.33333};
  size_t sz = sizeof(data)/sizeof(data[0]);
  printf("push to queue (sz = %lu)\n", sz);
  for (int i = 0; i < sz; ++i) {
    p1w_enqueue(q, gtype_d(data[i]));
    printf("%lf\n", data[i]);
  }
  printf("pop from queue\n");
  while (!p1w_is_empty(q)) {
    gtype elem = p1w_peek(q);
    printf("%lf\n", elem.d);
    p1w_dequeue(q);
  }
  p1w_free(q);
  return 0;
}