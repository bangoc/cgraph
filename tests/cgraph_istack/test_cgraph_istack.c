#include <stdio.h>

#include "cgraph_istack.h"
#include "ut.h"

int main() {
  cgraph_istack_t s = cgraph_istack_create();
  for (int elem = 1; elem < 10; ++elem) {
    cgraph_istack_push(s, elem);
  }
  bool any = false;
  if (cgraph_istack_size(s) != 9) {
    UT_MSG_FAILED("Stack size");
    any = true;
  }
  CGRAPH_INTEGER elem;
  if (cgraph_istack_top(s, &elem) != 0) {
    UT_MSG_FAILED("top failed");
    any = true;
  }
  if (elem != 9) {
    UT_MSG_FAILED("top failed (value)");
    any = true;
  }
  for (int i = 9; i >= 1; --i) {
    if (cgraph_istack_pop(s, &elem) != 0) {
      UT_MSG_FAILED("pop failed");
      any = true;
      break;
    }
    if (cgraph_istack_size(s) != i - 1) {
      UT_MSG_FAILED("size after pop");
      any = true;
      break;
    }
    if (elem != i) {
      UT_MSG_FAILED("pop failed (value)");
      any = true;
      break;
    }
  }
  if (any) {
    return 1;
  }
  cgraph_istack_free(&s);
  UT_MSG_OK("Test stack");
  return 0;
}