#include <stdio.h>

#include "cgraph.h"
#include "ut.h"

int main() {
  cgraph_t g;
  cgraph_ivec_t edges = cgraph_ivec_fromarray(
        (CGRAPH_INTEGER[]){ 0, 1, 
                            0, 3,
                            1, 2,
                            1, 3,
                            2, 4,
                            0, 2,
                            3, 5,
                            3, 1,
                            1, 4}, 18);
  cgraph_create(&g, edges, 0, true);
  if (!cgraph_ivec_equal(g.from, (CGRAPH_INTEGER[]){0, 0, 1, 1, 2, 0, 3, 3, 1})) {
    UT_MSG_FAILED("Test from vector");
    return 1;
  }
  if (!cgraph_ivec_equal(g.to, (CGRAPH_INTEGER[]){1, 3, 2, 3, 4, 2, 5, 1, 4})) {
    UT_MSG_FAILED("Test from vector");
    return 1;
  }
  if (!cgraph_ivec_equal(g.oi, (CGRAPH_INTEGER[]){0, 5, 1, 2, 3, 8, 4, 7, 6})) {
    UT_MSG_FAILED("Test from vector");
    return 1;
  }
  if (!cgraph_ivec_equal(g.ii, (CGRAPH_INTEGER[]){0, 7, 5, 2, 1, 3, 8, 4, 6})) {
    UT_MSG_FAILED("Test from vector");
    return 1;
  }
  if (!cgraph_ivec_equal(g.os, (CGRAPH_INTEGER[]){0, 3, 6, 7, 9, 9, 9})) {
    UT_MSG_FAILED("Test from vector");
    return 1;
  }
  if (!cgraph_ivec_equal(g.is, (CGRAPH_INTEGER[]){0, 0, 2, 4, 6, 8, 9})) {
    UT_MSG_FAILED("Test from vector");
    return 1;
  }
  UT_MSG_OK("Test create graph");
  return 0;
}