#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "cgen/all.h"
#include "base/cgraph_error.h"
#include "gb.h"
#include "ut.h"

int main() {
  cgraph_set_error_handler(cgraph_error_print);
  cgraph_t g = test_create_g1();
  double w[] = {3, 30, 5, 5, 10, 12, 11, 1, 6};
  int n = sizeof(w) / sizeof(double);
  arr_make(weights, 0, CGRAPH_REAL);
  for (int i = 0; i < n; ++i) {
    arr_append(weights, w[i]);
  }
  // g1-weighted.png

  atype(CGRAPH_INTEGER) *to = arr_ifrom_array((CGRAPH_INTEGER[]){1, 2, 3, 4, 5}, 5);
  struct paths *res = cgraph_get_shortest_paths_dijkstra(g, 0, to, weights, CGRAPH_OUT);
  bool any = false;
  if (!arr_iequal(res->vertices[0], (CGRAPH_INTEGER[]){0, 1}, 2)) {
    arr_iprint(res->vertices[0]);
    UT_MSG_FAILED("Case 1. Test vertices sequence 0->1 (out)");
    any = true;
  }
  if (!arr_iequal(res->edges[0], (CGRAPH_INTEGER[]){0}, 1)) {
    arr_iprint(res->edges[0]);
    UT_MSG_FAILED("Case 1. Test edges sequence 0->1 (out)");
    any = true;
  }
  if (!arr_iequal(res->vertices[1], (CGRAPH_INTEGER[]){0, 1, 2}, 3)) {
    arr_iprint(res->vertices[1]);
    UT_MSG_FAILED("Case 1. Test vertices sequence 0->2 (out)");
    any = true;
  }
  if (!arr_iequal(res->edges[1], (CGRAPH_INTEGER[]){0, 2}, 2)) {
    arr_iprint(res->edges[1]);
    UT_MSG_FAILED("Case 1. Test edges sequence 0->2 (out)");
    any = true;
  }
  if (!arr_iequal(res->vertices[2], (CGRAPH_INTEGER[]){0, 1, 3}, 3)) {
    arr_iprint(res->vertices[2]);
    UT_MSG_FAILED("Case 1. Test vertices sequence 0->3 (out)");
    any = true;
  }
  if (!arr_iequal(res->edges[2], (CGRAPH_INTEGER[]){0, 3}, 2)) {
    arr_iprint(res->edges[2]);
    UT_MSG_FAILED("Case 1. Test edges sequence 0->3 (out)");
    any = true;
  }
  if (!arr_iequal(res->vertices[3], (CGRAPH_INTEGER[]){0, 1, 4}, 3)) {
    arr_iprint(res->vertices[3]);
    UT_MSG_FAILED("Case 1. Test vertices sequence 0->4 (out)");
    any = true;
  }
  if (!arr_iequal(res->edges[3], (CGRAPH_INTEGER[]){0, 8}, 2)) {
    arr_iprint(res->edges[3]);
    UT_MSG_FAILED("Case 1. Test edges sequence 0->4 (out)");
    any = true;
  }
  if (!arr_iequal(res->vertices[4], (CGRAPH_INTEGER[]){0, 1, 3, 5}, 4)) {
    arr_iprint(res->vertices[4]);
    UT_MSG_FAILED("Case 1. Test vertices sequence 0->5 (out)");
    any = true;
  }
  if (!arr_iequal(res->edges[4], (CGRAPH_INTEGER[]){0, 3, 6}, 3)) {
    arr_iprint(res->edges[4]);
    UT_MSG_FAILED("Case 1. Test edges sequence 0->5 (out)");
    any = true;
  }
  free_paths(res);

  // Case 2: Not reach
  cgraph_set_warning_handler(cgraph_warning_print);
  arr_resize(to, 0);
  arr_append(to, 0);
  arr_append(to, 1);
  arr_append(to, 2);
  res = cgraph_get_shortest_paths_dijkstra(g, 1, to, weights, CGRAPH_OUT);
  if (res->predecessors[0] != -1) {
    UT_MSG_FAILED("Case 2. 0 not reached");
    any = true;
  }
  if (res->predecessors[1] != 1) {
    UT_MSG_FAILED("Case 2. Nguồn = Đích");
    any = true;
  }
  free_paths(res);
  arr_free(weights);
  arr_free(to);
  cgraph_destroy(g);
  if (any) {
    return 1;
  }
  UT_MSG_OK("Test dijkstra");
  return 0;
}
