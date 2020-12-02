#include <stdio.h>

#include "cgraph_iqueue.h"
#include "ut.h"

int main() {
  cgraph_iqueue_t q = cgraph_iqueue_create();
  for (int elem = 1; elem <= 10; ++elem) {
    cgraph_iqueue_enqueue(q, elem);
  }
  if (cgraph_iqueue_size(q) != 10) {
    UT_MSG_FAILED("Queue size");
    return 1;
  }
  CGRAPH_INTEGER elem;
  if (cgraph_iqueue_peek(q, &elem) != 0) {
    UT_MSG_FAILED("Peek failed");
    return 1;
  }
  if (elem != 1) {
    UT_MSG_FAILED("FIFO: peek");
    return 1;
  }
  for (int i = 1; i <= 10; ++i) {
    int ret = cgraph_iqueue_poll(q, &elem);
    if (cgraph_iqueue_size(q) != (10 - i) ||
        ret != 0 ||
        elem != i) {
      UT_MSG_FAILED("FIFO: poll");
      return 1;
    }
  }
  UT_MSG_OK("Queue test");
  cgraph_iqueue_free(&q);
  return 0;
}