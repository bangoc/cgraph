#include "cgen/cgen.h"

#include "gb.h"
#include "cgen_ext.h"
#include "ut.h"

bool t1() {
  cgraph_t g = test_create_g1();
  cgraph_ivec_t to = cgraph_ivec_from_array((CGRAPH_INTEGER[]){4, 5}, 2);
  gvec_t vertices = gvec_create(10, gtype_free_ivec_ref),
           edges = gvec_create(10, gtype_free_ivec_ref);
  for (int i = 0; i < cgraph_ivec_size(to); ++i) {
    gvec_append(vertices, gtype_v(cgraph_ivec_create_ref()));
    gvec_append(edges, gtype_v(cgraph_ivec_create_ref()));
  }
  cgraph_ivec_t predecessors = cgraph_ivec_create();
  cgraph_ivec_t inbound_edges = cgraph_ivec_create();
  cgraph_get_shortest_paths(g, vertices,
        edges, 0, to, CGRAPH_OUT, &predecessors, &inbound_edges);
  bool any = false;
  if (!cgraph_ivec_equal(ivec_at(vertices, 0), (CGRAPH_INTEGER[]){0, 2, 4}, 3) &&
      !cgraph_ivec_equal(ivec_at(vertices, 0), (CGRAPH_INTEGER[]){0, 1, 4}, 3)) {
    cgraph_ivec_print(ivec_at(vertices, 0));
    UT_MSG_FAILED("Case 1. Test vertices sequence 0->4");
    any = true;
  }
  if (!cgraph_ivec_equal(ivec_at(vertices, 1), (CGRAPH_INTEGER[]){0, 3, 5}, 3)) {
    cgraph_ivec_print(ivec_at(vertices, 1));
    UT_MSG_FAILED("Case 2. Test vertices sequence 0->5");
    any = true;
  }
  if (!cgraph_ivec_equal(ivec_at(edges, 0), (CGRAPH_INTEGER[]){5, 4}, 2) &&
      !cgraph_ivec_equal(ivec_at(edges, 0), (CGRAPH_INTEGER[]){0, 8}, 2)) {
    cgraph_ivec_print(ivec_at(edges, 0));
    UT_MSG_FAILED("Case 1. Test edges sequence 0->4");
    any = true;
  }
  if (!cgraph_ivec_equal(ivec_at(edges, 1), (CGRAPH_INTEGER[]){1, 6}, 2)) {
    cgraph_ivec_print(ivec_at(edges, 1));
    UT_MSG_FAILED("Case 2. Test edges sequence 0->5");
    any = true;
  }

  // Giải phóng bộ nhớ
  gvec_free(vertices);
  gvec_free(edges);
  cgraph_ivec_free(&predecessors);
  cgraph_ivec_free(&inbound_edges);
  cgraph_ivec_free(&to);
  cgraph_destroy(&g);
  return any;
}

bool t2() {
  cgraph_t g = test_create_g1();
  cgraph_ivec_t to = cgraph_ivec_from_array((CGRAPH_INTEGER[]){0, 4, 1}, 3);
  gvec_t vertices = gvec_create(5, gtype_free_ivec_ref),
           edges = gvec_create(5, gtype_free_ivec_ref);
  for (int i = 0; i < cgraph_ivec_size(to); ++i) {
    gvec_append(vertices, gtype_v(cgraph_ivec_create_ref()));
    gvec_append(edges, gtype_v(cgraph_ivec_create_ref()));
  }
  cgraph_ivec_t predecessors = cgraph_ivec_create();
  cgraph_ivec_t inbound_edges = cgraph_ivec_create();
  cgraph_get_shortest_paths(g, vertices,
        edges, 1, to, CGRAPH_OUT, &predecessors, &inbound_edges);
  bool any = false;
  if (predecessors[0] != -1 || predecessors[1] != 1) {
    cgraph_ivec_print(ivec_at(vertices, 0));
    UT_MSG_FAILED("Test 2. 0 is not reached, 1 is the start vertex");
    any = true;
  }
  if (!cgraph_ivec_equal(ivec_at(vertices, 1), (CGRAPH_INTEGER[]){1, 4}, 2)) {
    cgraph_ivec_print(ivec_at(vertices, 1));
    UT_MSG_FAILED("Test 2. Vertices sequence 1->4");
    any = true;
  }
  if (!cgraph_ivec_equal(ivec_at(edges, 1), (CGRAPH_INTEGER[]){8}, 1)) {
    cgraph_ivec_print(ivec_at(edges, 1));
    UT_MSG_FAILED("Test 2. Edges equence 8");
    any = true;
  }

  gvec_free(vertices);
  gvec_free(edges);
  cgraph_ivec_free(&predecessors);
  cgraph_ivec_free(&inbound_edges);
  cgraph_ivec_free(&to);
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