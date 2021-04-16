#include <stdio.h>

#include "gb.h"
#include "ut.h"

int main() {
  cgraph_t g = test_create_g1();
#define TEST_VEC(vec, arr, n, msg) \
{ \
  if (!cgraph_ivec_equal(vec, arr, n)) { \
    UT_MSG_FAILED(msg); \
    failed = true; \
  } \
}

#define TEST_GENERAL(g, vcount, ecount, directed, name) \
{ \
  if (cgraph_vcount(g) != vcount) { \
    UT_MSG_FAILED(name"Count vertices"); \
    failed = true; \
  } \
  if (cgraph_ecount(g) != ecount) { \
    UT_MSG_FAILED(name"Count edges"); \
    failed = true; \
  } \
  if (cgraph_is_directed(g) != directed) { \
    UT_MSG_FAILED(name"directed"); \
    failed = true; \
  } \
}

  bool failed = false;
  TEST_VEC(g->from,  ((CGRAPH_INTEGER[]){0, 0, 1, 1, 2, 0, 3, 3, 1}), 9, "g1.from");
  TEST_VEC(g->to,    ((CGRAPH_INTEGER[]){1, 3, 2, 3, 4, 2, 5, 1, 4}), 9, "g1.to");
  TEST_VEC(g->oi,    ((CGRAPH_INTEGER[]){0, 5, 1, 2, 3, 8, 4, 7, 6}), 9, "g1.oi");
  TEST_VEC(g->ii,    ((CGRAPH_INTEGER[]){0, 7, 5, 2, 1, 3, 8, 4, 6}), 9, "g1.ii");
  TEST_VEC(g->os,    ((CGRAPH_INTEGER[]){0, 3, 6, 7, 9, 9, 9}), 7, "g1.os");
  TEST_VEC(g->is,    ((CGRAPH_INTEGER[]){0, 0, 2, 4, 6, 8, 9}), 7, "g1.is");
  cgraph_destroy(&g);

  g = test_create_g2();
  TEST_VEC(g->from,  ((CGRAPH_INTEGER[]){0, 0, 1, 1, 2, 0, 3, 3, 1, 2}), 10, "g2.from");
  TEST_VEC(g->to,    ((CGRAPH_INTEGER[]){1, 3, 2, 3, 4, 2, 5, 1, 4, 2}), 10, "g2.to");
  TEST_VEC(g->oi,    ((CGRAPH_INTEGER[]){0, 5, 1, 2, 3, 8, 9, 4, 7, 6}), 10, "g2.oi");
  TEST_VEC(g->ii,    ((CGRAPH_INTEGER[]){0, 7, 5, 2, 9, 1, 3, 8, 4, 6}), 10, "g2.ii");
  TEST_VEC(g->os,    ((CGRAPH_INTEGER[]){0, 3, 6, 8, 10, 10, 10}), 7, "g2.os");
  TEST_VEC(g->is,    ((CGRAPH_INTEGER[]){0, 0, 2, 5, 7, 9, 10}), 7, "g2.is");
  cgraph_destroy(&g);

  g = test_create_g3();
  TEST_VEC(g->from,  ((CGRAPH_INTEGER[]){0, 0, 1, 1, 2, 0, 3, 1}), 8, "g3.from");
  TEST_VEC(g->to,    ((CGRAPH_INTEGER[]){1, 3, 2, 3, 4, 2, 5, 4}), 8, "g3.to");
  TEST_VEC(g->oi,    ((CGRAPH_INTEGER[]){0, 5, 1, 2, 3, 7, 4, 6}), 8, "g3.oi");
  TEST_VEC(g->ii,    ((CGRAPH_INTEGER[]){0, 5, 2, 1, 3, 7, 4, 6}), 8, "g3.ii");
  TEST_VEC(g->os,    ((CGRAPH_INTEGER[]){0, 3, 6, 7, 8, 8, 8}), 7, "g3.os");
  TEST_VEC(g->is,    ((CGRAPH_INTEGER[]){0, 0, 1, 3, 5, 7, 8}), 7, "g3.is");
  cgraph_destroy(&g);

  g = test_create_g4();
  TEST_GENERAL(g, 6, 8, false, "g4");
  TEST_VEC(g->from,  ((CGRAPH_INTEGER[]){1, 3, 2, 3, 4, 2, 5, 4}), 8, "g4.from");
  TEST_VEC(g->to,    ((CGRAPH_INTEGER[]){0, 0, 1, 1, 2, 0, 3, 1}), 8, "g4.to");
  TEST_VEC(g->oi,    ((CGRAPH_INTEGER[]){0, 5, 2, 1, 3, 7, 4, 6}), 8, "g4.oi");
  TEST_VEC(g->ii,    ((CGRAPH_INTEGER[]){0, 5, 1, 2, 3, 7, 4, 6}), 8, "g4.ii");
  TEST_VEC(g->os,    ((CGRAPH_INTEGER[]){0, 0, 1, 3, 5, 7, 8}), 7, "g4.os");
  TEST_VEC(g->is,    ((CGRAPH_INTEGER[]){0, 3, 6, 7, 8, 8, 8}), 7, "g4.is");
  cgraph_destroy(&g);

  g = test_create_g5();
  TEST_GENERAL(g, 6, 9, false, "g5");
  TEST_VEC(g->from,  ((CGRAPH_INTEGER[]){1, 3, 2, 3, 4, 2, 5, 4, 2}), 9, "g4.from");
  TEST_VEC(g->to,    ((CGRAPH_INTEGER[]){0, 0, 1, 1, 2, 0, 3, 1, 2}), 9, "g4.to");
  TEST_VEC(g->oi,    ((CGRAPH_INTEGER[]){0, 5, 2, 8, 1, 3, 7, 4, 6}), 9, "g4.oi");
  TEST_VEC(g->ii,    ((CGRAPH_INTEGER[]){0, 5, 1, 2, 3, 7, 8, 4, 6}), 9, "g4.ii");
  TEST_VEC(g->os,    ((CGRAPH_INTEGER[]){0, 0, 1, 4, 6, 8, 9}), 7, "g4.os");
  TEST_VEC(g->is,    ((CGRAPH_INTEGER[]){0, 3, 6, 8, 9, 9, 9}), 7, "g4.is");
  cgraph_destroy(&g);

#undef TEST_VEC
#undef TEST_GENERAL
  if (failed) {
    return 1;
  }
  UT_MSG_OK("Test create graph");
  return 0;
}