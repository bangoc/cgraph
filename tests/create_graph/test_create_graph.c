#include <stdio.h>

#include "gb.h"
#include "ut.h"

int main() {
  cgraph_t g;
  test_create_g1(&g);
  if (!cgraph_ivec_equal(g.from, (CGRAPH_INTEGER[]){0, 0, 1, 1, 2, 0, 3, 3, 1}, 9)) {
    UT_MSG_FAILED("Test from vector");
    return 1;
  }
  if (!cgraph_ivec_equal(g.to, (CGRAPH_INTEGER[]){1, 3, 2, 3, 4, 2, 5, 1, 4}, 9)) {
    UT_MSG_FAILED("Test from vector");
    return 1;
  }
  if (!cgraph_ivec_equal(g.oi, (CGRAPH_INTEGER[]){0, 5, 1, 2, 3, 8, 4, 7, 6}, 9)) {
    UT_MSG_FAILED("Test from vector");
    return 1;
  }
  if (!cgraph_ivec_equal(g.ii, (CGRAPH_INTEGER[]){0, 7, 5, 2, 1, 3, 8, 4, 6}, 9)) {
    UT_MSG_FAILED("Test from vector");
    return 1;
  }
  if (!cgraph_ivec_equal(g.os, (CGRAPH_INTEGER[]){0, 3, 6, 7, 9, 9, 9}, 7)) {
    UT_MSG_FAILED("Test from vector");
    return 1;
  }
  if (!cgraph_ivec_equal(g.is, (CGRAPH_INTEGER[]){0, 0, 2, 4, 6, 8, 9}, 7)) {
    UT_MSG_FAILED("Test from vector");
    return 1;
  }
  cgraph_destroy(&g);
  UT_MSG_OK("Test create graph");
  return 0;
}