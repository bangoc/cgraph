#include <stdio.h>

#include "gb.h"
#include "ut.h"

int main() {
  cgraph_t g;
  test_create_g1(&g);
  bool ret = false, failed = false;
  cgraph_is_dag(&g, &ret);
  if (ret != false) {
    UT_MSG_FAILED("Not DAG (g1)");
    failed = true;
  }
  
  cgraph_destroy(&g);
  test_create_g2(&g);
  cgraph_is_dag(&g, &ret);
  if (ret != false) {
    UT_MSG_FAILED("DAG (g2)");
    failed = true;
  }

  cgraph_destroy(&g);
  test_create_g3(&g);
  cgraph_is_dag(&g, &ret);
  if (ret != true) {
    UT_MSG_FAILED("DAG (g3)");
    failed = true;
  }

  cgraph_destroy(&g);
  test_create_g4(&g);
  cgraph_is_dag(&g, &ret);
  if (ret != false) {
    UT_MSG_FAILED("DAG (g4)");
    failed = true;
  }

  cgraph_destroy(&g);
  test_create_g5(&g);
  cgraph_is_dag(&g, &ret);
  if (ret != false) {
    UT_MSG_FAILED("DAG (g5)");
    failed = true;
  }

  if (failed) {
    return 1;
  }
  UT_MSG_OK("Is Dag");
  return 0;
}