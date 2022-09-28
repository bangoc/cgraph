#include <stdio.h>

#include "cgraph.h"
#include "gb.h"
#include "ut.h"

int main() {
  cgraph_t g = test_create_g2();
  bool failed = false;


#define TEST_ONE(res, expected, msg) \
{ \
  if (res != expected) { \
    UT_MSG_FAILED(msg); \
    printf("%d\n", res); \
    failed = true; \
  } \
}

#define TEST_ALL(vec, arr, n, msg) \
  { \
    if (!arr_iequal(vec, arr, n)) { \
      UT_MSG_FAILED(msg); \
      arr_iprint(vec); \
      failed = true; \
    } \
  }

  CGRAPH_INTEGER d = 0;
  d = cgraph_degree_one(g, 1, CGRAPH_OUT, false);
  TEST_ONE(d, 3, "g2. Test one - out degree");
  d = cgraph_degree_one(g, 1, CGRAPH_IN, false);
  TEST_ONE(d, 2, "g2. Test one - in degree");
  d = cgraph_degree_one(g, 1, CGRAPH_ALL, false);
  TEST_ONE(d, 5, "g2. Test one - in+out degree");

  atype(CGRAPH_INTEGER) *degree = NULL;
  degree = cgraph_degree_all(g, CGRAPH_OUT, false);
  TEST_ALL(degree, ((CGRAPH_INTEGER[]){3, 3, 1, 2, 0, 0}), 6, "g2. All out degree (no loop)");
  arr_free(degree);

  degree = cgraph_degree_all(g, CGRAPH_IN, false);
  TEST_ALL(degree, ((CGRAPH_INTEGER[]){0, 2, 2, 2, 2, 1}), 6, "g2. All in degree (no loop)");
  arr_free(degree);

  degree = cgraph_degree_all(g, CGRAPH_ALL, false);
  TEST_ALL(degree, ((CGRAPH_INTEGER[]){3, 5, 3, 4, 2, 1}), 6, "g2. All in+out degree (no loop");
  arr_free(degree);

  // with loop
  degree = cgraph_degree_all(g, CGRAPH_OUT, true);
  TEST_ALL(degree, ((CGRAPH_INTEGER[]){3, 3, 2, 2, 0, 0}), 6, "g2. All out degree (with loop)");
  arr_free(degree);

  degree = cgraph_degree_all(g, CGRAPH_IN, true);
  TEST_ALL(degree, ((CGRAPH_INTEGER[]){0, 2, 3, 2, 2, 1}), 6, "g2. All in degree (with loop)");
  arr_free(degree);

  degree = cgraph_degree_all(g, CGRAPH_ALL, true);
  TEST_ALL(degree, ((CGRAPH_INTEGER[]){3, 5, 5, 4, 2, 1}), 6, "g2. All in+out degree (with loop)");
  arr_free(degree);

  cgraph_destroy(g);

  // Test undirected graph with loop
  g = test_create_g5();

  d = cgraph_degree_one(g, 2, CGRAPH_OUT, false);
  TEST_ONE(d, 3, "g5. Test one - out degree");
  d = cgraph_degree_one(g, 2, CGRAPH_IN, false);
  TEST_ONE(d, 3, "g5. Test one - in degree");
  d = cgraph_degree_one(g, 2, CGRAPH_ALL, false);
  TEST_ONE(d, 3, "g5. Test one - in+out degree");

  d = cgraph_degree_one(g, 2, CGRAPH_OUT, true);
  TEST_ONE(d, 5, "g5. Test one - out degree");
  d = cgraph_degree_one(g, 2, CGRAPH_IN, true);
  TEST_ONE(d, 5, "g5. Test one - in degree");
  d = cgraph_degree_one(g, 2, CGRAPH_ALL, true);
  TEST_ONE(d, 5, "g5. Test one - in+out degree");

  degree = cgraph_degree_all(g, CGRAPH_OUT, false);
  TEST_ALL(degree, ((CGRAPH_INTEGER[]){3, 4, 3, 3, 2, 1}), 6, "g5. All out degree (no loop)");
  arr_free(degree);

  degree = cgraph_degree_all(g, CGRAPH_IN, false);
  TEST_ALL(degree, ((CGRAPH_INTEGER[]){3, 4, 3, 3, 2, 1}), 6, "g5. All in degree (no loop)");
  arr_free(degree);

  degree = cgraph_degree_all(g, CGRAPH_ALL, false);
  TEST_ALL(degree, ((CGRAPH_INTEGER[]){3, 4, 3, 3, 2, 1}), 6, "g5. All in+out degree (no loop");
  arr_free(degree);

  // with loop
  degree = cgraph_degree_all(g, CGRAPH_OUT, true);
  TEST_ALL(degree, ((CGRAPH_INTEGER[]){3, 4, 5, 3, 2, 1}), 6, "g5. All out degree (with loop)");
  arr_free(degree);

  degree = cgraph_degree_all(g, CGRAPH_IN, true);
  TEST_ALL(degree, ((CGRAPH_INTEGER[]){3, 4, 5, 3, 2, 1}), 6, "g5. All in degree (with loop)");
  arr_free(degree);

  degree = cgraph_degree_all(g, CGRAPH_ALL, true);
  TEST_ALL(degree, ((CGRAPH_INTEGER[]){3, 4, 5, 3, 2, 1}), 6, "g5. All in+out degree (with loop)");
  arr_free(degree);

  cgraph_destroy(g);
#undef TEST_ALL
#undef TEST_ONE
  if (failed) {
    return 1;
  }
  UT_MSG_OK("Test degree");
  return 0;
}
