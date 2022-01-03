#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "cgen/cgen.h"
#include "cgraph_error.h"
#include "gb.h"
#include "cgen_ext.h"
#include "ut.h"

int main() {
  cgraph_set_error_handler(cgraph_error_print);
  cgraph_t g = test_create_g1();
  double w[] = {3, 30, 5, 5, 10, 12, 11, 1, 6};
  int n = sizeof(w) / sizeof(double);
  cgraph_rvec_t weights = cgraph_rvec_create();
  for (int i = 0; i < n; ++i) {
    cgraph_rvec_push_back(&weights, w[i]);
  }
  // g1-weighted.png

  cgraph_ivec_t to = cgraph_ivec_from_array((CGRAPH_INTEGER[]){1, 2, 3, 4, 5}, 5);
  gvec_t vertices = gvec_create(10, gtype_free_ivec_ref),
         edges = gvec_create(10, gtype_free_ivec_ref);
  for (int i = 0; i < cgraph_ivec_size(to); ++i) {
    gvec_append(vertices, gtype_v(cgraph_ivec_create_ref()));
    gvec_append(edges, gtype_v(cgraph_ivec_create_ref()));
  }
  cgraph_ivec_t predecessors = cgraph_ivec_create();
  cgraph_ivec_t inbound_edges = cgraph_ivec_create();

  cgraph_get_shortest_paths_dijkstra(g, vertices,
        edges, 0, to, weights, CGRAPH_OUT, &predecessors, &inbound_edges);
  bool any = false;
  if (!cgraph_ivec_equal(ivec_at(vertices, 0), (CGRAPH_INTEGER[]){0, 1}, 2)) {
    cgraph_ivec_print(ivec_at(vertices, 0));
    UT_MSG_FAILED("Case 1. Test vertices sequence 0->1 (out)");
    any = true;
  }
  if (!cgraph_ivec_equal(ivec_at(edges, 0), (CGRAPH_INTEGER[]){0}, 1)) {
    cgraph_ivec_print(ivec_at(edges, 0));
    UT_MSG_FAILED("Case 1. Test edges sequence 0->1 (out)");
    any = true;
  }
  if (!cgraph_ivec_equal(ivec_at(vertices, 1), (CGRAPH_INTEGER[]){0, 1, 2}, 3)) {
    cgraph_ivec_print(ivec_at(vertices, 1));
    UT_MSG_FAILED("Case 1. Test vertices sequence 0->2 (out)");
    any = true;
  }
  if (!cgraph_ivec_equal(ivec_at(edges, 1), (CGRAPH_INTEGER[]){0, 2}, 2)) {
    cgraph_ivec_print(ivec_at(edges, 1));
    UT_MSG_FAILED("Case 1. Test edges sequence 0->2 (out)");
    any = true;
  }
  if (!cgraph_ivec_equal(ivec_at(vertices, 2), (CGRAPH_INTEGER[]){0, 1, 3}, 3)) {
    cgraph_ivec_print(ivec_at(vertices, 2));
    UT_MSG_FAILED("Case 1. Test vertices sequence 0->3 (out)");
    any = true;
  }
  if (!cgraph_ivec_equal(ivec_at(edges, 2), (CGRAPH_INTEGER[]){0, 3}, 2)) {
    cgraph_ivec_print(ivec_at(edges, 2));
    UT_MSG_FAILED("Case 1. Test edges sequence 0->3 (out)");
    any = true;
  }
  if (!cgraph_ivec_equal(ivec_at(vertices, 3), (CGRAPH_INTEGER[]){0, 1, 4}, 3)) {
    cgraph_ivec_print(ivec_at(vertices, 3));
    UT_MSG_FAILED("Case 1. Test vertices sequence 0->4 (out)");
    any = true;
  }
  if (!cgraph_ivec_equal(ivec_at(edges, 3), (CGRAPH_INTEGER[]){0, 8}, 2)) {
    cgraph_ivec_print(ivec_at(edges, 3));
    UT_MSG_FAILED("Case 1. Test edges sequence 0->4 (out)");
    any = true;
  }
  if (!cgraph_ivec_equal(ivec_at(vertices, 4), (CGRAPH_INTEGER[]){0, 1, 3, 5}, 4)) {
    cgraph_ivec_print(ivec_at(vertices, 4));
    UT_MSG_FAILED("Case 1. Test vertices sequence 0->5 (out)");
    any = true;
  }
  if (!cgraph_ivec_equal(ivec_at(edges, 4), (CGRAPH_INTEGER[]){0, 3, 6}, 3)) {
    cgraph_ivec_print(ivec_at(edges, 4));
    UT_MSG_FAILED("Case 1. Test edges sequence 0->5 (out)");
    any = true;
  }
  gvec_free(vertices);
  gvec_free(edges);

  // Case 2: Not reach
  cgraph_set_warning_handler(cgraph_warning_print);
  cgraph_ivec_init(&to, 0);
  cgraph_ivec_push_back(&to, 0);
  cgraph_ivec_push_back(&to, 1);
  cgraph_ivec_push_back(&to, 2);
  vertices = gvec_create(10, gtype_free_ivec_ref);
  edges = gvec_create(10, gtype_free_ivec_ref);
  for (int i = 0; i < cgraph_ivec_size(to); ++i) {
    gvec_append(vertices, gtype_v(cgraph_ivec_create_ref()));
    gvec_append(edges, gtype_v(cgraph_ivec_create_ref()));
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
  cgraph_ivec_free(&predecessors);
  cgraph_ivec_free(&inbound_edges);
  cgraph_rvec_free(&weights);
  cgraph_ivec_free(&to);
  cgraph_destroy(&g);
  if (any) {
    return 1;
  }
  UT_MSG_OK("Test dijkstra");
  return 0;
}
