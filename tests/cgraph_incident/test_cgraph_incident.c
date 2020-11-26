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
  cgraph_ivec_t v = cgraph_ivec_create();
  cgraph_incident(&g, &v, 1, CGRAPH_IN);
  if (!cgraph_ivec_equal(v, (CGRAPH_INTEGER[]){0, 7}, 2)) {
    UT_MSG_FAILED("Test IN Incident");
    return 1;
  }
  cgraph_incident(&g, &v, 1, CGRAPH_OUT);
  if (!cgraph_ivec_equal(v, (CGRAPH_INTEGER[]){2, 3, 8}, 3)) {
    UT_MSG_FAILED("Test OUT Incident");
    return 1;
  }
  cgraph_incident(&g, &v, 1, CGRAPH_ALL);
  if (!cgraph_ivec_equal(v, (CGRAPH_INTEGER[]){2, 3, 8, 0, 7}, 5)) {
    UT_MSG_FAILED("Test ALL Incident");
    return 1;
  }
  UT_MSG_OK("Test neightbors");
  return 0;
}