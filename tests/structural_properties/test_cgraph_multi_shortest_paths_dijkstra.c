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
  gvec_t vertices = gvec_create_full(0, 10, gtype_zero, gtype_free_iarr_ref),
         edges = gvec_create_full(0, 10, gtype_zero, gtype_free_iarr_ref);
  for (int i = 0; i < arr_size(to); ++i) {
    gvec_append(vertices, gtype_v(arr_icreate_ref()));
    gvec_append(edges, gtype_v(arr_icreate_ref()));
  }
  arr_make(predecessors, 0, CGRAPH_INTEGER);
  arr_make(inbound_edges, 0, CGRAPH_INTEGER);

  cgraph_get_shortest_paths_dijkstra(g, vertices,
        edges, 0, to, weights, CGRAPH_OUT, &predecessors, &inbound_edges);
  bool any = false;
  if (!arr_iequal(arr_iptr_at(vertices, 0), (CGRAPH_INTEGER[]){0, 1}, 2)) {
    arr_iprint(arr_iptr_at(vertices, 0));
    UT_MSG_FAILED("Case 1. Test vertices sequence 0->1 (out)");
    any = true;
  }
  if (!arr_iequal(arr_iptr_at(edges, 0), (CGRAPH_INTEGER[]){0}, 1)) {
    arr_iprint(arr_iptr_at(edges, 0));
    UT_MSG_FAILED("Case 1. Test edges sequence 0->1 (out)");
    any = true;
  }
  if (!arr_iequal(arr_iptr_at(vertices, 1), (CGRAPH_INTEGER[]){0, 1, 2}, 3)) {
    arr_iprint(arr_iptr_at(vertices, 1));
    UT_MSG_FAILED("Case 1. Test vertices sequence 0->2 (out)");
    any = true;
  }
  if (!arr_iequal(arr_iptr_at(edges, 1), (CGRAPH_INTEGER[]){0, 2}, 2)) {
    arr_iprint(arr_iptr_at(edges, 1));
    UT_MSG_FAILED("Case 1. Test edges sequence 0->2 (out)");
    any = true;
  }
  if (!arr_iequal(arr_iptr_at(vertices, 2), (CGRAPH_INTEGER[]){0, 1, 3}, 3)) {
    arr_iprint(arr_iptr_at(vertices, 2));
    UT_MSG_FAILED("Case 1. Test vertices sequence 0->3 (out)");
    any = true;
  }
  if (!arr_iequal(arr_iptr_at(edges, 2), (CGRAPH_INTEGER[]){0, 3}, 2)) {
    arr_iprint(arr_iptr_at(edges, 2));
    UT_MSG_FAILED("Case 1. Test edges sequence 0->3 (out)");
    any = true;
  }
  if (!arr_iequal(arr_iptr_at(vertices, 3), (CGRAPH_INTEGER[]){0, 1, 4}, 3)) {
    arr_iprint(arr_iptr_at(vertices, 3));
    UT_MSG_FAILED("Case 1. Test vertices sequence 0->4 (out)");
    any = true;
  }
  if (!arr_iequal(arr_iptr_at(edges, 3), (CGRAPH_INTEGER[]){0, 8}, 2)) {
    arr_iprint(arr_iptr_at(edges, 3));
    UT_MSG_FAILED("Case 1. Test edges sequence 0->4 (out)");
    any = true;
  }
  if (!arr_iequal(arr_iptr_at(vertices, 4), (CGRAPH_INTEGER[]){0, 1, 3, 5}, 4)) {
    arr_iprint(arr_iptr_at(vertices, 4));
    UT_MSG_FAILED("Case 1. Test vertices sequence 0->5 (out)");
    any = true;
  }
  if (!arr_iequal(arr_iptr_at(edges, 4), (CGRAPH_INTEGER[]){0, 3, 6}, 3)) {
    arr_iprint(arr_iptr_at(edges, 4));
    UT_MSG_FAILED("Case 1. Test edges sequence 0->5 (out)");
    any = true;
  }
  gvec_free(vertices);
  gvec_free(edges);

  // Case 2: Not reach
  cgraph_set_warning_handler(cgraph_warning_print);
  arr_resize(to, 0);
  arr_append(to, 0);
  arr_append(to, 1);
  arr_append(to, 2);
  vertices = gvec_create_full(0, 10, gtype_zero, gtype_free_iarr_ref);
  edges = gvec_create_full(0, 10, gtype_zero, gtype_free_iarr_ref);
  for (int i = 0; i < arr_size(to); ++i) {
    gvec_append(vertices, gtype_v(arr_icreate_ref()));
    gvec_append(edges, gtype_v(arr_icreate_ref()));
  }
  cgraph_get_shortest_paths_dijkstra(g, vertices,
        edges, 1, to, weights, CGRAPH_OUT, &predecessors, &inbound_edges);
  if (predecessors[0] != -1) {
    UT_MSG_FAILED("Case 2. 0 not reached");
    any = true;
  }
  if (predecessors[1] != 1) {
    UT_MSG_FAILED("Case 2. Nguồn = Đích");
    any = true;
  }
  gvec_free(vertices);
  gvec_free(edges);
  arr_free(predecessors);
  arr_free(inbound_edges);
  arr_free(weights);
  arr_free(to);
  cgraph_destroy(g);
  if (any) {
    return 1;
  }
  UT_MSG_OK("Test dijkstra");
  return 0;
}
