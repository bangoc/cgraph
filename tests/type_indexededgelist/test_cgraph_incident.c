#include <stdio.h>

#include "gb.h"
#include "ut.h"

int main() {
  cgraph_t g = test_create_g1();
  atype(CGRAPH_INTEGER) *v = NULL;
  v = cgraph_incident(g, 1, CGRAPH_IN);
  if (!arr_iequal(v, (CGRAPH_INTEGER[]){0, 7}, 2)) {
    UT_MSG_FAILED("Test IN Incident");
    return 1;
  }
  arr_free(v);

  v = cgraph_incident(g, 1, CGRAPH_OUT);
  if (!arr_iequal(v, (CGRAPH_INTEGER[]){2, 3, 8}, 3)) {
    UT_MSG_FAILED("Test OUT Incident");
    return 1;
  }
  arr_free(v);

  v = cgraph_incident(g, 1, CGRAPH_ALL);
  if (!arr_iequal(v, (CGRAPH_INTEGER[]){2, 3, 8, 0, 7}, 5)) {
    UT_MSG_FAILED("Test ALL Incident");
    return 1;
  }
  arr_free(v);

  UT_MSG_OK("Test neightbors");
  cgraph_destroy(&g);
  return 0;
}