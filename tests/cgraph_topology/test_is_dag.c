#include <stdio.h>

#include "gb.h"
#include "ut.h"

int main() {
  cgraph_t g;
  test_create_g1(&g);
  bool ret = false, failed = false;
  cgraph_is_dag(&g, &ret);
  if (ret == true) {
    UT_MSG_FAILED("Not DAG (t1)");
    failed = true;
  }
  cgraph_t g3;
  test_create_g3(&g3);
  cgraph_is_dag(&g3, &ret);
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