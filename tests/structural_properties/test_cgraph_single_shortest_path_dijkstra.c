#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "gb.h"
#include "ut.h"

int main() {
  cgraph_t g = test_create_g1();
  double w[] = {3, 30, 5, 5, 10, 12, 11, 1, 6};
  int n = sizeof(w) / sizeof(double);
  arr_make(weights, 0, CGRAPH_REAL);
  for (int i = 0; i < n; ++i) {
    arr_append(weights, w[i]);
  }
  // g1-weighted.png

  bool any = false;
  struct path *p = cgraph_get_shortest_path_dijkstra(g, 0, 5, weights, CGRAPH_OUT);
  if (!p->reached) {
    UT_MSG_FAILED("Can reach 5 from 0");
    any = true;
  }
  if (!arr_iequal(p->vertices, (CGRAPH_INTEGER[]){0, 1, 3, 5}, 4)) {
    arr_iprint(p->vertices);
    UT_MSG_FAILED("Case 1. Test vertices sequence 0-5 (out)");
    any = true;
  }
  if (!arr_iequal(p->edges, (CGRAPH_INTEGER[]){0, 3, 6}, 3)) {
    arr_iprint(p->edges);
    UT_MSG_FAILED("Case 1. Test edges sequence 0-5 (out)");
    any = true;
  }
  path_free(p);

  p = cgraph_get_shortest_path_dijkstra(g, 0, 4, weights, CGRAPH_OUT);
  if (!p->reached) {
    UT_MSG_FAILED("Can reach 4 from 0");
    any = true;
  }

  if (!arr_iequal(p->vertices, (CGRAPH_INTEGER[]){0, 1, 4}, 3)) {
    arr_iprint(p->vertices);
    UT_MSG_FAILED("Case 2. Test vertices sequence 0-4 (out)");
    any = true;
  }
  if (!arr_iequal(p->edges, (CGRAPH_INTEGER[]){0, 8}, 2)) {
    arr_iprint(p->edges);
    UT_MSG_FAILED("Case 2. Test edges sequence 0-4 (out)");
    any = true;
  }
  path_free(p);

  p = cgraph_get_shortest_path_dijkstra(g, 2, 3, weights, CGRAPH_IN);
  if (!p->reached) {
    UT_MSG_FAILED("Can reach 3 from 2");
    any = true;
  }

  if (!arr_iequal(p->vertices, (CGRAPH_INTEGER[]){2, 1, 3}, 3)) {
    arr_iprint(p->vertices);
    UT_MSG_FAILED("Case 3. Test vertices sequence 2-3 (in)");
    any = true;
  }
  if (!arr_iequal(p->edges, (CGRAPH_INTEGER[]){2, 7}, 2)) {
    arr_iprint(p->edges);
    UT_MSG_FAILED("Case 3. Test edges sequence 2-3 (in)");
    any = true;
  }
  path_free(p);

  p = cgraph_get_shortest_path_dijkstra(g, 4, 5, weights, CGRAPH_ALL);
  if (!p->reached) {
    UT_MSG_FAILED("Can reach 5 from 4");
    any = true;
  }
  if (!arr_iequal(p->vertices, (CGRAPH_INTEGER[]){4, 1, 3, 5}, 4)) {
    arr_iprint(p->vertices);
    UT_MSG_FAILED("Case 4. Test vertices sequence 4-5 (all)");
    any = true;
  }
  if (!arr_iequal(p->edges, (CGRAPH_INTEGER[]){8, 7, 6}, 3)) {
    arr_iprint(p->edges);
    UT_MSG_FAILED("Case 4. Test edges sequence 4-5 (all)");
    any = true;
  }
  path_free(p);

  p = cgraph_get_shortest_path_dijkstra(g, 3, 0, weights, CGRAPH_OUT);
  if (p->reached) {
    UT_MSG_FAILED("Can't reach 0 from 3");
    any = true;
  }
  path_free(p);

  p = cgraph_get_shortest_path_dijkstra(g, 3, 3, weights, CGRAPH_OUT);
  if (!p->reached) {
    UT_MSG_FAILED("Can reach 3 from 3");
    any = true;
  }
  if (!arr_iequal(p->vertices, (CGRAPH_INTEGER[]){3}, 1)) {
    arr_iprint(p->vertices);
    UT_MSG_FAILED("Case 6. p->vertices contains to if from==to, 3-3 (out)");
    any = true;
  }
  if (arr_size(p->edges) > 0) {
    arr_iprint(p->edges);
    UT_MSG_FAILED("Case 6. p->edges empty if from==to, 3-3 (out)");
    any = true;
  }
  path_free(p);

  p = cgraph_get_shortest_path_dijkstra(g, 0, 5, NULL, CGRAPH_OUT);
  if (!p->reached) {
    UT_MSG_FAILED("Can reach 5 from 0");
    any = true;
  }
  if (!arr_iequal(p->vertices, (CGRAPH_INTEGER[]){0, 3, 5}, 3)) {
    arr_iprint(p->vertices);
    UT_MSG_FAILED("Case 7. No weight 0-5 (out)");
    any = true;
  }
  if (!arr_iequal(p->edges, (CGRAPH_INTEGER[]){1, 6}, 2)) {
    arr_iprint(p->edges);
    UT_MSG_FAILED("Case 7. No weight 0-5 (out)");
    any = true;
  }
  path_free(p);

  /* crash test in the no path case*/
  p = cgraph_get_shortest_path_dijkstra(g, 3, 0, weights, CGRAPH_OUT);
  path_free(p);

  cgraph_destroy(g);
  arr_free(weights);
  if (any) {
    return 1;
  }
  UT_MSG_OK("Test dijkstra");

  return 0;
}