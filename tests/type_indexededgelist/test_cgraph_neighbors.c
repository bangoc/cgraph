#include <stdio.h>

#include "gb.h"
#include "ut.h"

int main() {
  cgraph_t g = test_create_g1();
  arr_make(v, 0, CGRAPH_INTEGER);
  cgraph_neighbors(g, &v, 1, CGRAPH_IN);
  if (!arr_iequal(v, (CGRAPH_INTEGER[]){0, 3}, 2)) {
    arr_iprint(v);
    UT_MSG_FAILED("Test IN Neighbors");
    return 1;
  }
  cgraph_neighbors(g, &v, 1, CGRAPH_OUT);
  if (!arr_iequal(v, (CGRAPH_INTEGER[]){2, 3, 4}, 3)) {
    arr_iprint(v);
    UT_MSG_FAILED("Test OUT Neighbors");
    return 1;
  }
  cgraph_neighbors(g, &v, 1, CGRAPH_ALL);
  if (!arr_iequal(v, (CGRAPH_INTEGER[]){0, 2, 3, 3, 4}, 5)) {
    arr_iprint(v);
    UT_MSG_FAILED("Test ALL Neighbors");
    return 1;
  }
  UT_MSG_OK("Test neightbors");
  return 0;
}