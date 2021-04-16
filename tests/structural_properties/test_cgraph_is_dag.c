#include <stdio.h>

#include "gb.h"
#include "ut.h"

int main() {
  cgraph_t g = test_create_g1();
  bool ret = false, failed = false;
  ret = cgraph_is_dag(g);
  if (ret != false) {
    UT_MSG_FAILED("Not DAG (g1)");
    failed = true;
  }

  cgraph_destroy(&g);
  g = test_create_g2();
  ret = cgraph_is_dag(g);
  if (ret != false) {
    UT_MSG_FAILED("DAG (g2)");
    failed = true;
  }

  cgraph_destroy(&g);
  g = test_create_g3();
  ret = cgraph_is_dag(g);
  if (ret != true) {
    UT_MSG_FAILED("DAG (g3)");
    failed = true;
  }

  cgraph_destroy(&g);
  g = test_create_g4();
  ret = cgraph_is_dag(g);
  if (ret != false) {
    UT_MSG_FAILED("DAG (g4)");
    failed = true;
  }

  cgraph_destroy(&g);
  g = test_create_g5(g);
  ret = cgraph_is_dag(g);
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