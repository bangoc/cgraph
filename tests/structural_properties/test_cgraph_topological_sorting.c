#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "gb.h"
#include "ut.h"

bool is_valid_topological_order(cgraph_t g,
                                atype(CGRAPH_INTEGER) *v,
                                cgraph_neimode_t mode) {
  cgraph_neimode_t deg_mode;
  if (mode == CGRAPH_IN) {
    deg_mode = CGRAPH_OUT;
  } else if (mode == CGRAPH_OUT) {
    deg_mode = CGRAPH_IN;
  }
  CGRAPH_INTEGER no_of_nodes = cgraph_vcount(g);
  arr_make(neis, 0, CGRAPH_INTEGER);
  atype(CGRAPH_INTEGER) *degrees = cgraph_degree_all(g, deg_mode, true);

  CGRAPH_INTEGER sz = arr_size(v);
  bool *removed = calloc(no_of_nodes, sizeof(bool));
  bool any = false;
  for (CGRAPH_INTEGER i = 0; i < sz; ++i) {
    if (degrees[ v[i] ] != 0 || removed[ v[i] ]) {
      any = true;
      break;
    }
    removed[ v[i] ] = true;
    CGRAPH_INTEGER node = v[i];
    cgraph_neighbors(g, &neis, node, mode);
    for (CGRAPH_INTEGER i = 0; i < arr_size(neis); i++) {
      CGRAPH_INTEGER nei = neis[i];
      degrees[nei]--;
    }
  }
  free(removed);
  arr_free(degrees);
  arr_free(neis);
  if (any) {
    return false;
  }
  return true;
}

int main() {
  cgraph_t g = test_create_g3();
  bool any = false;
  arr_make(res, 0, CGRAPH_INTEGER);
  /*
  out: {0, 1, 2, 3, 4, 5}
  in: {4, 5, 2, 3, 1, 0}
  */
  cgraph_topological_sorting(g, &res, CGRAPH_OUT);
  if (!is_valid_topological_order(g, res, CGRAPH_OUT) || arr_size(res) != 6) {
    UT_MSG_FAILED("Out topological sorting on G3");
    any = true;
  }
  cgraph_topological_sorting(g, &res, CGRAPH_IN);
  if (!is_valid_topological_order(g, res, CGRAPH_IN) || arr_size(res) != 6) {
    UT_MSG_FAILED("In topological sorting on G3");
    any = true;
  }
  arr_free(res);
  cgraph_destroy(&g);
  if (any) {
    return 1;
  }
  UT_MSG_OK("Test topological sorting");
  return 0;
}