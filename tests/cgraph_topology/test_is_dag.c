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
  bool ret = false, failed = false;
  cgraph_is_dag(&g, &ret);
  if (ret == true) {
    UT_MSG_FAILED("Not DAG (t1)");
    failed = true;
  }
  cgraph_t g2;
  cgraph_ivec_t edges2 = cgraph_ivec_fromarray(
        (CGRAPH_INTEGER[]){ 0, 1, 
                            0, 3,
                            1, 2,
                            1, 3,
                            2, 4,
                            0, 2,
                            3, 5,
                            1, 4}, 16);
  cgraph_create(&g2, edges2, 0, true);
  cgraph_is_dag(&g2, &ret);
  if (ret == false) {
    UT_MSG_FAILED("DAG (t3)");
    failed = true;
  }
  if (failed) {
    return 1;
  }
  UT_MSG_OK("Is Dag");
  return 0;
}