#include <stdio.h>
#include <stdlib.h>
#include "ctc/pqueue.h"

int cmp_double(const void *o1, const void *o2) {
  double val1 = *((const double*)o1),
         val2 = *((const double*)o2);
  return (val1 > val2) - (val2 > val1);
}

double *clone_double(double value) {
  double *p = malloc(sizeof(double));
  *p = value;
  return p;
}

int main() {
  PQueue *q;
  pqueue_new(&q, cmp_double);
  double data[] = {0.111, 2.22222, 3.3333, -5.5555,
                   9.9999, -1.1111, 10.10101, -3.33333};
  size_t sz = sizeof(data)/sizeof(data[0]);
  printf("push to queue (sz = %lu)\n", sz);
  for (int i = 0; i < sz; ++i) {
    pqueue_push(q, clone_double(data[i]));
    printf("%lf\n", data[i]);
  }
  double *elem;
  printf("pop from queue\n");
  while (pqueue_pop(q, (void**)&elem) == CC_OK) {
    printf("%lf\n", *elem);
    free(elem);
  }
  pqueue_destroy(q);
  return 0;
}