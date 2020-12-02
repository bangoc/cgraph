#include <stdio.h>

#include "cgraph_ivec.h"
#include "ut.h"

#define N 100000

void foo(cgraph_ivec_t *v) {
  for (int i = 0; i < N; ++i) {
    cgraph_ivec_push_back(v, i);
  }
}

int main() {
  cgraph_ivec_t v = cgraph_ivec_create();
  foo(&v);
  for (int i = 0; i < N; ++i) {
    if (v[i] != i) {
      UT_MSG_FAILED("Value different.");
      return 1;
    }
  }
  UT_MSG_OK("Pass cgraph_ivec_t as value and add elements");
  return 0;
}