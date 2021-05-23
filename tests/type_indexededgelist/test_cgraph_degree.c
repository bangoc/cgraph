#include <stdio.h>

#include "cgraph.h"
#include "gb.h"
#include "ut.h"

int main() {
  cgraph_t g = test_create_g2();
  cgraph_ivec_t degree = cgraph_ivec_create();
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
    if (!cgraph_ivec_equal(vec, arr, n)) { \
      UT_MSG_FAILED(msg); \
      print_ivec(vec); \
      failed = true; \
    } \
  }

  CGRAPH_INTEGER d = 0;
  cgraph_degree_one(g, &d, 1, CGRAPH_OUT, false);
  TEST_ONE(d, 3, "g2. Test one - out degree");
  cgraph_degree_one(g, &d, 1, CGRAPH_IN, false);
  TEST_ONE(d, 2, "g2. Test one - in degree");
  cgraph_degree_one(g, &d, 1, CGRAPH_ALL, false);
  TEST_ONE(d, 5, "g2. Test one - in+out degree");

  cgraph_degree_all(g, &degree, CGRAPH_OUT, false);
  TEST_ALL(degree, ((CGRAPH_INTEGER[]){3, 3, 1, 2, 0, 0}), 6, "g2. All out degree (no loop)");
  cgraph_degree_all(g, &degree, CGRAPH_IN, false);
  TEST_ALL(degree, ((CGRAPH_INTEGER[]){0, 2, 2, 2, 2, 1}), 6, "g2. All in degree (no loop)");
  cgraph_degree_all(g, &degree, CGRAPH_ALL, false);
  TEST_ALL(degree, ((CGRAPH_INTEGER[]){3, 5, 3, 4, 2, 1}), 6, "g2. All in+out degree (no loop");

  // with loop
  cgraph_degree_all(g, &degree, CGRAPH_OUT, true);
  TEST_ALL(degree, ((CGRAPH_INTEGER[]){3, 3, 2, 2, 0, 0}), 6, "g2. All out degree (with loop)");
  cgraph_degree_all(g, &degree, CGRAPH_IN, true);
  TEST_ALL(degree, ((CGRAPH_INTEGER[]){0, 2, 3, 2, 2, 1}), 6, "g2. All in degree (with loop)");
  cgraph_degree_all(g, &degree, CGRAPH_ALL, true);
  TEST_ALL(degree, ((CGRAPH_INTEGER[]){3, 5, 5, 4, 2, 1}), 6, "g2. All in+out degree (with loop)");

  cgraph_destroy(&g);

  // Test undirected graph with loop
  g = test_create_g5();

  cgraph_degree_one(g, &d, 2, CGRAPH_OUT, false);
  TEST_ONE(d, 3, "g5. Test one - out degree");
  cgraph_degree_one(g, &d, 2, CGRAPH_IN, false);
  TEST_ONE(d, 3, "g5. Test one - in degree");
  cgraph_degree_one(g, &d, 2, CGRAPH_ALL, false);
  TEST_ONE(d, 3, "g5. Test one - in+out degree");

  cgraph_degree_one(g, &d, 2, CGRAPH_OUT, true);
  TEST_ONE(d, 5, "g5. Test one - out degree");
  cgraph_degree_one(g, &d, 2, CGRAPH_IN, true);
  TEST_ONE(d, 5, "g5. Test one - in degree");
  cgraph_degree_one(g, &d, 2, CGRAPH_ALL, true);
  TEST_ONE(d, 5, "g5. Test one - in+out degree");

  cgraph_degree_all(g, &degree, CGRAPH_OUT, false);
  TEST_ALL(degree, ((CGRAPH_INTEGER[]){3, 4, 3, 3, 2, 1}), 6, "g5. All out degree (no loop)");
  cgraph_degree_all(g, &degree, CGRAPH_IN, false);
  TEST_ALL(degree, ((CGRAPH_INTEGER[]){3, 4, 3, 3, 2, 1}), 6, "g5. All in degree (no loop)");
  cgraph_degree_all(g, &degree, CGRAPH_ALL, false);
  TEST_ALL(degree, ((CGRAPH_INTEGER[]){3, 4, 3, 3, 2, 1}), 6, "g5. All in+out degree (no loop");

  // with loop
  cgraph_degree_all(g, &degree, CGRAPH_OUT, true);
  TEST_ALL(degree, ((CGRAPH_INTEGER[]){3, 4, 5, 3, 2, 1}), 6, "g5. All out degree (with loop)");
  cgraph_degree_all(g, &degree, CGRAPH_IN, true);
  TEST_ALL(degree, ((CGRAPH_INTEGER[]){3, 4, 5, 3, 2, 1}), 6, "g5. All in degree (with loop)");
  cgraph_degree_all(g, &degree, CGRAPH_ALL, true);
  TEST_ALL(degree, ((CGRAPH_INTEGER[]){3, 4, 5, 3, 2, 1}), 6, "g5. All in+out degree (with loop)");
  cgraph_degree_all(g, &degree, CGRAPH_OUT, true);
  cgraph_destroy(&g);
#undef TEST_ALL
#undef TEST_ONE
  if (failed) {
    return 1;
  }
  UT_MSG_OK("Test degree");
  return 0;
}