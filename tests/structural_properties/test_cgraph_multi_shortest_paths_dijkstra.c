#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "cgen/gtvector.h"
#include "cgraph_error.h"
#include "gb.h"
#include "ut.h"

int main() {
  cgraph_t g = test_create_g1();
  double w[] = {3, 30, 5, 5, 10, 12, 11, 1, 6};
  int n = sizeof(w) / sizeof(double);
  cgraph_rvec_t weights = cgraph_rvec_create();
  for (int i = 0; i < n; ++i) {
    cgraph_rvec_push_back(&weights, w[i]);
  }
  // g1-weighted.png

  cgraph_ivec_t to = cgraph_ivec_from_array((CGRAPH_INTEGER[]){1, 2, 3, 4, 5}, 5);
  vector_t vertices = gtv_create(),
           edges = gtv_create();
  for (int i = 0; i < cgraph_ivec_size(to); ++i) {
    gtv_push_back(&vertices, (gtype){.v = cgraph_ivec_create()});
    gtv_push_back(&edges, (gtype){.v = cgraph_ivec_create()});
  }
  cgraph_ivec_t predecessors = cgraph_ivec_create();
  cgraph_ivec_t inbound_edges = cgraph_ivec_create();

  cgraph_get_shortest_paths_dijkstra(g, vertices,
        edges, 0, to, weights, CGRAPH_OUT, &predecessors, &inbound_edges);
  bool any = false;
  if (!cgraph_ivec_equal(vertices[0].v, (CGRAPH_INTEGER[]){0, 1}, 2)) {
    cgraph_ivec_print(vertices[0].v);
    UT_MSG_FAILED("Case 1. Test vertices sequence 0->1 (out)");
    any = true;
  }
  if (!cgraph_ivec_equal(edges[0].v, (CGRAPH_INTEGER[]){0}, 1)) {
    cgraph_ivec_print(vertices[0].v);
    UT_MSG_FAILED("Case 1. Test edges sequence 0->1 (out)");
    any = true;
  }
  if (!cgraph_ivec_equal(vertices[1].v, (CGRAPH_INTEGER[]){0, 1, 2}, 3)) {
    cgraph_ivec_print(vertices[1].v);
    UT_MSG_FAILED("Case 1. Test vertices sequence 0->2 (out)");
    any = true;
  }
  if (!cgraph_ivec_equal(edges[1].v, (CGRAPH_INTEGER[]){0, 2}, 2)) {
    cgraph_ivec_print(vertices[1].v);
    UT_MSG_FAILED("Case 1. Test edges sequence 0->2 (out)");
    any = true;
  }
  if (!cgraph_ivec_equal(vertices[2].v, (CGRAPH_INTEGER[]){0, 1, 3}, 3)) {
    cgraph_ivec_print(vertices[2].v);
    UT_MSG_FAILED("Case 1. Test vertices sequence 0->3 (out)");
    any = true;
  }
  if (!cgraph_ivec_equal(edges[2].v, (CGRAPH_INTEGER[]){0, 3}, 2)) {
    cgraph_ivec_print(vertices[2].v);
    UT_MSG_FAILED("Case 1. Test edges sequence 0->3 (out)");
    any = true;
  }
  if (!cgraph_ivec_equal(vertices[3].v, (CGRAPH_INTEGER[]){0, 1, 4}, 3)) {
    cgraph_ivec_print(vertices[3].v);
    UT_MSG_FAILED("Case 1. Test vertices sequence 0->4 (out)");
    any = true;
  }
  if (!cgraph_ivec_equal(edges[3].v, (CGRAPH_INTEGER[]){0, 8}, 2)) {
    cgraph_ivec_print(vertices[3].v);
    UT_MSG_FAILED("Case 1. Test edges sequence 0->4 (out)");
    any = true;
  }
  if (!cgraph_ivec_equal(vertices[4].v, (CGRAPH_INTEGER[]){0, 1, 3, 5}, 4)) {
    cgraph_ivec_print(vertices[4].v);
    UT_MSG_FAILED("Case 1. Test vertices sequence 0->5 (out)");
    any = true;
  }
  if (!cgraph_ivec_equal(edges[4].v, (CGRAPH_INTEGER[]){0, 3, 6}, 3)) {
    cgraph_ivec_print(vertices[4].v);
    UT_MSG_FAILED("Case 1. Test edges sequence 0->5 (out)");
    any = true;
  }

  // Case 2: Not reach
  cgraph_set_warning_handler(cgraph_warning_print);
  cgraph_ivec_init(&to, 0);
  cgraph_ivec_push_back(&to, 0);
  cgraph_ivec_push_back(&to, 1);
  cgraph_ivec_push_back(&to, 2);
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

  for (int i = 0; i < gtv_size(vertices); ++i) {
    cgraph_ivec_free(&vertices[i].v);
  }
  for (int i = 0; i < gtv_size(edges); ++i) {
    cgraph_ivec_free(&edges[i].v);
  }
  gtv_free(&vertices);
  gtv_free(&edges);
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