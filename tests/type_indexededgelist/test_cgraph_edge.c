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
  CGRAPH_INTEGER from, to;
  bool any = false;
  for (int eid = 0; eid < cgraph_ecount(g); ++eid) {
    cgraph_edge(g, eid, &from, &to);
    if (from != edges[2 * eid] || to != edges[2 * eid + 1]) {
      UT_MSG_FAILED("Case 1. Edge from -> to");
      any = true;
      break;
    }
  }
  cgraph_destroy(&g);
  cgraph_ivec_free(&edges);
  g = test_create_g4();

  // from <= to on the output
  edges = cgraph_ivec_from_array(
        (CGRAPH_INTEGER[]){ 0, 1,
                            0, 3,
                            1, 2,
                            1, 3,
                            2, 4,
                            0, 2,
                            3, 5,
                            1, 4}, 16);
  for (int eid = 0; eid < cgraph_ecount(g); ++eid) {
    cgraph_edge(g, eid, &from, &to);
    if (from != edges[2 * eid] || to != edges[2 * eid + 1]) {
      UT_MSG_FAILED("Case 2. Undirected from -- to");
      fprintf(stderr, "%d %d vs %d %d\n", from, to, edges[2 * eid], edges[2 * eid + 1]);
      any = true;
      break;
    }
  }

  if (any) {
    UT_MSG_FAILED("Test edge");
    return 1;
  }
  UT_MSG_OK("Test edge");
  return 0;
}