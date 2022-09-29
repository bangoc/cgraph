#include "cgen/all.h"

#include "gb.h"
#include "ut.h"

bool t1() {
  cgraph_t g = test_create_g1();
  atype(CGRAPH_INTEGER) *to = arr_ifrom_array((CGRAPH_INTEGER[]){4, 5}, 2);
  struct paths *res = cgraph_get_shortest_paths(g, 0, to, CGRAPH_OUT);
  bool any = false;
  if (!arr_iequal(res->vertices[0], (CGRAPH_INTEGER[]){0, 2, 4}, 3) &&
      !arr_iequal(res->vertices[0], (CGRAPH_INTEGER[]){0, 1, 4}, 3)) {
    arr_iprint(res->vertices[0]);
    UT_MSG_FAILED("Case 1. Test vertices sequence 0->4");
    any = true;
  }
  if (!arr_iequal(res->vertices[1], (CGRAPH_INTEGER[]){0, 3, 5}, 3)) {
    arr_iprint(res->vertices[1]);
    UT_MSG_FAILED("Case 2. Test vertices sequence 0->5");
    any = true;
  }
  if (!arr_iequal(res->edges[0], (CGRAPH_INTEGER[]){5, 4}, 2) &&
      !arr_iequal(res->edges[0], (CGRAPH_INTEGER[]){0, 8}, 2)) {
    arr_iprint(res->edges[0]);
    UT_MSG_FAILED("Case 1. Test edges sequence 0->4");
    any = true;
  }
  if (!arr_iequal(res->edges[1], (CGRAPH_INTEGER[]){1, 6}, 2)) {
    arr_iprint(res->edges[1]);
    UT_MSG_FAILED("Case 2. Test edges sequence 0->5");
    any = true;
  }

  // Giải phóng bộ nhớ
  arr_free(to);
  paths_free(res);
  cgraph_destroy(g);
  return any;
}

bool t2() {
  cgraph_t g = test_create_g1();
  atype(CGRAPH_INTEGER) *to = arr_ifrom_array((CGRAPH_INTEGER[]){0, 4, 1}, 3);
  struct paths *res = cgraph_get_shortest_paths(g, 1, to, CGRAPH_OUT);
  bool any = false;
  if (res->predecessors[0] != -1 || res->predecessors[1] != 1) {
    arr_iprint(res->vertices[0]);
    UT_MSG_FAILED("Test 2. 0 is not reached, 1 is the start vertex");
    any = true;
  }
  if (!arr_iequal(res->vertices[1], (CGRAPH_INTEGER[]){1, 4}, 2)) {
    arr_iprint(res->vertices[1]);
    UT_MSG_FAILED("Test 2. Vertices sequence 1->4");
    any = true;
  }
  if (!arr_iequal(res->edges[1], (CGRAPH_INTEGER[]){8}, 1)) {
    arr_iprint(res->edges[1]);
    UT_MSG_FAILED("Test 2. Edges equence 8");
    any = true;
  }

  paths_free(res);
  arr_free(to);
  cgraph_destroy(g);
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