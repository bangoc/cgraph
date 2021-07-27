#include "cgraph.h"
#include "tests/base/gb.h"

int main() {
  cgraph_t g1 = test_create_g1();
  cgraph_write_graph_dot(g1, stdout);
  cgraph_destroy(&g1);
  return 0;
}