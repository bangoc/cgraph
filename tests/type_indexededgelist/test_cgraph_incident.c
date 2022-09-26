#include <stdio.h>

#include "gb.h"
#include "ut.h"

int main() {
  cgraph_t g = test_create_g1();
  arr_make(v, 0, CGRAPH_INTEGER);
  cgraph_incident(g, &v, 1, CGRAPH_IN);
  if (!arr_iequal(v, (CGRAPH_INTEGER[]){0, 7}, 2)) {
    UT_MSG_FAILED("Test IN Incident");
    return 1;
  }
  cgraph_incident(g, &v, 1, CGRAPH_OUT);
  if (!arr_iequal(v, (CGRAPH_INTEGER[]){2, 3, 8}, 3)) {
    UT_MSG_FAILED("Test OUT Incident");
    return 1;
  }
  cgraph_incident(g, &v, 1, CGRAPH_ALL);
  if (!arr_iequal(v, (CGRAPH_INTEGER[]){2, 3, 8, 0, 7}, 5)) {
    UT_MSG_FAILED("Test ALL Incident");
    return 1;
  }
  UT_MSG_OK("Test neightbors");
  arr_free(v);
  cgraph_destroy(&g);
  return 0;
}