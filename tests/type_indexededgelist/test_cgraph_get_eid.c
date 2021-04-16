#include <stdio.h>

#include "gb.h"
#include "ut.h"

int main() {
  cgraph_t g = test_create_g1();
  cgraph_ivec_t edges = cgraph_ivec_from_array(
      (CGRAPH_INTEGER[]){ 0, 1,
                          0, 3,
                          1, 2,
                          1, 3,
                          2, 4,
                          0, 2,
                          3, 5,
                          3, 1,
                          1, 4}, 18);
  bool any = false;
  CGRAPH_INTEGER id = 0, id2 = 0;
  for (int eid = 0; eid < cgraph_ecount(g); ++eid) {
    cgraph_get_eid(g, &id, edges[eid * 2], edges[eid * 2 + 1], true);
    if (id != eid) {
      UT_MSG_FAILED("Case 1. Edge from -> to");
      any = true;
      break;
    }

    cgraph_get_eid(g, &id, edges[eid * 2], edges[eid * 2 + 1], false);
    cgraph_get_eid(g, &id2, edges[eid * 2], edges[eid * 2 + 1], false);
    if (id != eid || id2 != eid) {
      UT_MSG_FAILED("Case 2. Undirected from - to");
      fprintf(stderr, "%d %d vs %d\n", id, id2, eid);
      any = true;
      break;
    }
  }
  cgraph_get_eid(g, &id, 1, 10, true);
  cgraph_get_eid(g, &id2, 2, 15, false);
  if (id != -1 || id2 != -1) {
    UT_MSG_FAILED("Case 3. No edge");
    any = true;
  }
  if (any) {
    UT_MSG_FAILED("Test edge");
    return 1;
  }
  UT_MSG_OK("Test edge");
  return 0;
}