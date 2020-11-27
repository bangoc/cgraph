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
  CGRAPH_INTEGER d = 0;
  cgraph_degree_one(&g, &d, 1, CGRAPH_OUT, false);
  TEST_ONE(d, 3, "Test one - out degree");
  cgraph_degree_one(&g, &d, 1, CGRAPH_IN, false);
  TEST_ONE(d, 2, "Test one - in degree");
  cgraph_degree_one(&g, &d, 1, CGRAPH_ALL, false);
  TEST_ONE(d, 5, "Test one - in+out degree");
#undef TEST_ONE

#define TEST_ALL(vec, arr, n, msg) \
  { \
    if (!cgraph_ivec_equal(vec, arr, n)) { \
      UT_MSG_FAILED(msg); \
      print_ivec(vec); \
      failed = true; \
    } \
  }
  cgraph_degree_all(&g, &degree, CGRAPH_OUT, false);
  TEST_ALL(degree, ((CGRAPH_INTEGER[]){3, 3, 1, 2, 0, 0}), 6, "All out degree");
  cgraph_degree_all(&g, &degree, CGRAPH_IN, false);
  TEST_ALL(degree, ((CGRAPH_INTEGER[]){0, 2, 2, 2, 2, 1}), 6, "All in degree");
  cgraph_degree_all(&g, &degree, CGRAPH_ALL, false);
  TEST_ALL(degree, ((CGRAPH_INTEGER[]){3, 5, 3, 4, 2, 1}), 6, "All in+out degree");
#undef TEST_ALL
  if (failed) {
    return 1;
  }
  UT_MSG_OK("Test degree");
  return 0;
}