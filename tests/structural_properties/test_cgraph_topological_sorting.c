#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "gb.h"
#include "ut.h"

bool is_valid_topological_order(cgraph_t *g,
                                cgraph_ivec_t const v,
                                cgraph_neimode_t mode) {
  cgraph_neimode_t deg_mode;
  if (mode == CGRAPH_IN) {
    deg_mode = CGRAPH_OUT;
  } else if (mode == CGRAPH_OUT) {
    deg_mode = CGRAPH_IN;
  }
  CGRAPH_INTEGER no_of_nodes = cgraph_vcount(g);
  cgraph_ivec_t degrees = cgraph_ivec_create(),
                neis = cgraph_ivec_create();
  cgraph_degree_all(g, &degrees, deg_mode, true);

  CGRAPH_INTEGER sz = cgraph_ivec_size(v);
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
    for (CGRAPH_INTEGER i = 0;
         i < cgraph_ivec_size(neis); i++) {
      CGRAPH_INTEGER nei = neis[i];
      degrees[nei]--;
    }
  }
  free(removed);
  cgraph_ivec_free(&degrees);
  cgraph_ivec_free(&neis);
  if (any) {
    return false;
  }
  return true;
}

int main() {
  cgraph_t g;
  test_create_g3(&g);
  bool any = false;
  cgraph_ivec_t res = cgraph_ivec_create();
  /*
  out: {0, 1, 2, 3, 4, 5}
  in: {4, 5, 2, 3, 1, 0}
  */
  cgraph_topological_sorting(&g, &res, CGRAPH_OUT);
  if (!is_valid_topological_order(&g, res, CGRAPH_OUT) || cgraph_ivec_size(res) != 6) {
    UT_MSG_FAILED("Out topological sorting on G3");
    any = true;
  }
  cgraph_topological_sorting(&g, &res, CGRAPH_IN);
  if (!is_valid_topological_order(&g, res, CGRAPH_IN) || cgraph_ivec_size(res) != 6) {
    UT_MSG_FAILED("In topological sorting on G3");
    any = true;
  }
  cgraph_ivec_free(&res);
  cgraph_destroy(&g);
  if (any) {
    return 1;
  }
  UT_MSG_OK("Test topological sorting");
  return 0;
}