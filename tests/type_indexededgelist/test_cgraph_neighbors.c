#include <stdio.h>

#include "gb.h"
#include "ut.h"

int main() {
  cgraph_t g = test_create_g1();
  cgraph_ivec_t v = cgraph_ivec_create();
  cgraph_neighbors(g, &v, 1, CGRAPH_IN);
  if (!cgraph_ivec_equal(v, (CGRAPH_INTEGER[]){0, 3}, 2)) {
    print_ivec(v);
    UT_MSG_FAILED("Test IN Neighbors");
    return 1;
  }
  cgraph_neighbors(g, &v, 1, CGRAPH_OUT);
  if (!cgraph_ivec_equal(v, (CGRAPH_INTEGER[]){2, 3, 4}, 3)) {
    print_ivec(v);
    UT_MSG_FAILED("Test OUT Neighbors");
    return 1;
  }
  cgraph_neighbors(g, &v, 1, CGRAPH_ALL);
  if (!cgraph_ivec_equal(v, (CGRAPH_INTEGER[]){0, 2, 3, 3, 4}, 5)) {
    print_ivec(v);
    UT_MSG_FAILED("Test ALL Neighbors");
    return 1;
  }
  UT_MSG_OK("Test neightbors");
  return 0;
}