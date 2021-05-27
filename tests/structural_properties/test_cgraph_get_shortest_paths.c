#include "cgen/gtvector.h"

#include "gb.h"
#include "ut.h"

bool t1() {
  cgraph_t g = test_create_g1();
  cgraph_ivec_t to = cgraph_ivec_from_array((CGRAPH_INTEGER[]){4, 5}, 2);
  vector_t vertices = gtv_create(),
           edges = gtv_create();
  for (int i = 0; i < cgraph_ivec_size(to); ++i) {
    gtv_push_back(&vertices, (gtype){.v = cgraph_ivec_create()});
    gtv_push_back(&edges, (gtype){.v = cgraph_ivec_create()});
  }
  cgraph_ivec_t predecessors = cgraph_ivec_create();
  cgraph_ivec_t inbound_edges = cgraph_ivec_create();
  cgraph_get_shortest_paths(g, vertices,
        edges, 0, to, CGRAPH_OUT, &predecessors, &inbound_edges);
  bool any = false;
  if (!cgraph_ivec_equal(vertices[0].v, (CGRAPH_INTEGER[]){0, 2, 4}, 3) &&
      !cgraph_ivec_equal(vertices[0].v, (CGRAPH_INTEGER[]){0, 1, 4}, 3)) {
    cgraph_ivec_print(vertices[0].v);
    UT_MSG_FAILED("Case 1. Test vertices sequence 0->4");
    any = true;
  }
  if (!cgraph_ivec_equal(vertices[1].v, (CGRAPH_INTEGER[]){0, 3, 5}, 3)) {
    cgraph_ivec_print(vertices[1].v);
    UT_MSG_FAILED("Case 2. Test vertices sequence 0->5");
    any = true;
  }
  if (!cgraph_ivec_equal(edges[0].v, (CGRAPH_INTEGER[]){5, 4}, 2) &&
      !cgraph_ivec_equal(edges[0].v, (CGRAPH_INTEGER[]){0, 8}, 2)) {
    cgraph_ivec_print(edges[0].v);
    UT_MSG_FAILED("Case 1. Test edges sequence 0->4");
    any = true;
  }
  if (!cgraph_ivec_equal(edges[1].v, (CGRAPH_INTEGER[]){1, 6}, 2)) {
    cgraph_ivec_print(edges[1].v);
    UT_MSG_FAILED("Case 2. Test edges sequence 0->5");
    any = true;
  }

  // Giải phóng bộ nhớ
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
  cgraph_ivec_free(&to);
  cgraph_destroy(&g);
  return any;
}

bool t2() {
  cgraph_t g = test_create_g1();
  cgraph_ivec_t to = cgraph_ivec_from_array((CGRAPH_INTEGER[]){0, 4, 1}, 3);
  vector_t vertices = gtv_create(),
           edges = gtv_create();
  for (int i = 0; i < cgraph_ivec_size(to); ++i) {
    gtv_push_back(&vertices, (gtype){.v = cgraph_ivec_create()});
    gtv_push_back(&edges, (gtype){.v = cgraph_ivec_create()});
  }
  cgraph_ivec_t predecessors = cgraph_ivec_create();
  cgraph_ivec_t inbound_edges = cgraph_ivec_create();
  cgraph_get_shortest_paths(g, vertices,
        edges, 1, to, CGRAPH_OUT, &predecessors, &inbound_edges);
  bool any = false;
  if (predecessors[0] != -1 || predecessors[1] != 1) {
    cgraph_ivec_print(vertices[0].v);
    UT_MSG_FAILED("Test 2. 0 is not reached, 1 is the start vertex");
    any = true;
  }
  if (!cgraph_ivec_equal(vertices[1].v, (CGRAPH_INTEGER[]){1, 4}, 2)) {
    cgraph_ivec_print(vertices[1].v);
    UT_MSG_FAILED("Test 2. Vertices sequence 1->4");
    any = true;
  }
  if (!cgraph_ivec_equal(edges[1].v, (CGRAPH_INTEGER[]){8}, 1)) {
    cgraph_ivec_print(edges[1].v);
    UT_MSG_FAILED("Test 2. Edges equence 8");
    any = true;
  }

  // Giải phóng bộ nhớ
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