#include <stdio.h>

#include "cgraph_ivec.h"
#include "ut.h"

#define N 100000

void foo(cgraph_ivec_t v) {
  for (int i = 0; i < N; ++i) {
    cgraph_ivec_push_back(&v, i);
  }
}

int main() {
  cgraph_ivec_t v = cgraph_ivec_create();
  foo(v);
  for (int i = 0; i < N; ++i) {
    if (v[i] != i) {
      UT_MSG_FAILED("Value different.");
      /*
      This is an intentionally failed case,
      i kept this test to demonstrate a wrong use of a vector.
      !Do not pass vector by value and then add elements to it,
      because of realloc can change the storage address.
      */
      return 0;
    }
  }
  UT_MSG_OK("Pass cgraph_ivec_t as value and add elements");
  return 0;
}