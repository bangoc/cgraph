#include "cgen/all.h"

#include "gb.h"
#include "cgen_ext.h"
#include "ut.h"

bool t1() {
  cgraph_t g = test_create_g1();
  arr_ptr(CGRAPH_INTEGER) to = arr_ifrom_array((CGRAPH_INTEGER[]){4, 5}, 2);
  gvec_t vertices = gvec_create_full(0, 10, gtype_zero, gtype_free_iarr_ref),
           edges = gvec_create_full(0, 10, gtype_zero, gtype_free_iarr_ref);
  for (int i = 0; i < arr_size(to); ++i) {
    gvec_append(vertices, gtype_v(arr_icreate_ref()));
    gvec_append(edges, gtype_v(arr_icreate_ref()));
  }
  arr_make(predecessors, 0, CGRAPH_INTEGER);
  arr_make(inbound_edges, 0, CGRAPH_INTEGER);
  cgraph_get_shortest_paths(g, vertices,
        edges, 0, to, CGRAPH_OUT, &predecessors, &inbound_edges);
  bool any = false;
  if (!arr_iequal(arr_iptr_at(vertices, 0), (CGRAPH_INTEGER[]){0, 2, 4}, 3) &&
      !arr_iequal(arr_iptr_at(vertices, 0), (CGRAPH_INTEGER[]){0, 1, 4}, 3)) {
    arr_iprint(arr_iptr_at(vertices, 0));
    UT_MSG_FAILED("Case 1. Test vertices sequence 0->4");
    any = true;
  }
  if (!arr_iequal(arr_iptr_at(vertices, 1), (CGRAPH_INTEGER[]){0, 3, 5}, 3)) {
    arr_iprint(arr_iptr_at(vertices, 1));
    UT_MSG_FAILED("Case 2. Test vertices sequence 0->5");
    any = true;
  }
  if (!arr_iequal(arr_iptr_at(edges, 0), (CGRAPH_INTEGER[]){5, 4}, 2) &&
      !arr_iequal(arr_iptr_at(edges, 0), (CGRAPH_INTEGER[]){0, 8}, 2)) {
    arr_iprint(arr_iptr_at(edges, 0));
    UT_MSG_FAILED("Case 1. Test edges sequence 0->4");
    any = true;
  }
  if (!arr_iequal(arr_iptr_at(edges, 1), (CGRAPH_INTEGER[]){1, 6}, 2)) {
    arr_iprint(arr_iptr_at(edges, 1));
    UT_MSG_FAILED("Case 2. Test edges sequence 0->5");
    any = true;
  }

  // Giải phóng bộ nhớ
  gvec_free(vertices);
  gvec_free(edges);
  arr_free(predecessors);
  arr_free(inbound_edges);
  arr_free(to);
  cgraph_destroy(&g);
  return any;
}

bool t2() {
  cgraph_t g = test_create_g1();
  arr_ptr(CGRAPH_INTEGER) to = arr_ifrom_array((CGRAPH_INTEGER[]){0, 4, 1}, 3);
  gvec_t vertices = gvec_create_full(0, 5, gtype_zero, gtype_free_iarr_ref),
           edges = gvec_create_full(0, 5, gtype_zero, gtype_free_iarr_ref);
  for (int i = 0; i < arr_size(to); ++i) {
    gvec_append(vertices, gtype_v(arr_icreate_ref()));
    gvec_append(edges, gtype_v(arr_icreate_ref()));
  }
  arr_make(predecessors, 0, CGRAPH_INTEGER);
  arr_make(inbound_edges, 0, CGRAPH_INTEGER);
  cgraph_get_shortest_paths(g, vertices,
        edges, 1, to, CGRAPH_OUT, &predecessors, &inbound_edges);
  bool any = false;
  if (predecessors[0] != -1 || predecessors[1] != 1) {
    arr_iprint(arr_iptr_at(vertices, 0));
    UT_MSG_FAILED("Test 2. 0 is not reached, 1 is the start vertex");
    any = true;
  }
  if (!arr_iequal(arr_iptr_at(vertices, 1), (CGRAPH_INTEGER[]){1, 4}, 2)) {
    arr_iprint(arr_iptr_at(vertices, 1));
    UT_MSG_FAILED("Test 2. Vertices sequence 1->4");
    any = true;
  }
  if (!arr_iequal(arr_iptr_at(edges, 1), (CGRAPH_INTEGER[]){8}, 1)) {
    arr_iprint(arr_iptr_at(edges, 1));
    UT_MSG_FAILED("Test 2. Edges equence 8");
    any = true;
  }

  gvec_free(vertices);
  gvec_free(edges);
  arr_free(predecessors);
  arr_free(inbound_edges);
  arr_free(to);
  cgraph_destroy(&g);
  return any;
}

int main() {
  bool any = t1() || t2();
  if (any) {
    return 1;
  }
  UT_MSG_OK("get shortest paths OK");
  return 0;
}