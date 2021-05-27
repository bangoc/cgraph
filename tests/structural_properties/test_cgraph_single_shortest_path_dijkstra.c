#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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

  cgraph_ivec_t vpath = cgraph_ivec_create(),
                epath = cgraph_ivec_create();
  bool any = false;
  if (cgraph_get_shortest_path_dijkstra(
      g, &vpath, &epath, 0, 5, weights, CGRAPH_OUT) != 0) {
    UT_MSG_FAILED("Return 0 if reached");
    any = true;
  }
  if (!cgraph_ivec_equal(vpath, (CGRAPH_INTEGER[]){0, 1, 3, 5}, 4)) {
    cgraph_ivec_print(vpath);
    UT_MSG_FAILED("Case 1. Test vertices sequence 0-5 (out)");
    any = true;
  }
  if (!cgraph_ivec_equal(epath, (CGRAPH_INTEGER[]){0, 3, 6}, 3)) {
    cgraph_ivec_print(epath);
    UT_MSG_FAILED("Case 1. Test edges sequence 0-5 (out)");
    any = true;
  }

  if (cgraph_get_shortest_path_dijkstra(g, &vpath, &epath, 0, 4, weights, CGRAPH_OUT) != 0) {
    UT_MSG_FAILED("Return 0 if reached");
    any = true;
  }

  if (!cgraph_ivec_equal(vpath, (CGRAPH_INTEGER[]){0, 1, 4}, 3)) {
    cgraph_ivec_print(vpath);
    UT_MSG_FAILED("Case 2. Test vertices sequence 0-4 (out)");
    any = true;
  }
  if (!cgraph_ivec_equal(epath, (CGRAPH_INTEGER[]){0, 8}, 2)) {
    cgraph_ivec_print(epath);
    UT_MSG_FAILED("Case 2. Test edges sequence 0-4 (out)");
    any = true;
  }

  if (cgraph_get_shortest_path_dijkstra(g, &vpath, &epath, 2, 3, weights, CGRAPH_IN) != 0) {
    UT_MSG_FAILED("Return 0 if reached");
    any = true;
  }

  if (!cgraph_ivec_equal(vpath, (CGRAPH_INTEGER[]){2, 1, 3}, 3)) {
    cgraph_ivec_print(vpath);
    UT_MSG_FAILED("Case 3. Test vertices sequence 2-3 (in)");
    any = true;
  }
  if (!cgraph_ivec_equal(epath, (CGRAPH_INTEGER[]){2, 7}, 2)) {
    cgraph_ivec_print(epath);
    UT_MSG_FAILED("Case 3. Test edges sequence 2-3 (in)");
    any = true;
  }

  if (cgraph_get_shortest_path_dijkstra(g, &vpath, &epath, 4, 5, weights, CGRAPH_ALL) != 0) {
    UT_MSG_FAILED("Return 0 if reached");
    any = true;
  }
  if (!cgraph_ivec_equal(vpath, (CGRAPH_INTEGER[]){4, 1, 3, 5}, 4)) {
    cgraph_ivec_print(vpath);
    UT_MSG_FAILED("Case 4. Test vertices sequence 4-5 (all)");
    any = true;
  }
  if (!cgraph_ivec_equal(epath, (CGRAPH_INTEGER[]){8, 7, 6}, 3)) {
    cgraph_ivec_print(epath);
    UT_MSG_FAILED("Case 4. Test edges sequence 4-5 (all)");
    any = true;
  }

  if (cgraph_get_shortest_path_dijkstra(g, &vpath, &epath, 3, 0, weights, CGRAPH_OUT) != -1) {
    UT_MSG_FAILED("Return -1 if not reached");
    any = true;
  }

  if (cgraph_get_shortest_path_dijkstra(g, &vpath, &epath, 3, 3, weights, CGRAPH_OUT) != 0) {
    UT_MSG_FAILED("Return 0 if reached");
    any = true;
  }
  if (!cgraph_ivec_equal(vpath, (CGRAPH_INTEGER[]){3}, 1)) {
    cgraph_ivec_print(vpath);
    UT_MSG_FAILED("Case 6. vpath contains to if from==to, 3-3 (out)");
    any = true;
  }
  if (cgraph_ivec_size(epath) > 0) {
    cgraph_ivec_print(epath);
    UT_MSG_FAILED("Case 6. epath empty if from==to, 3-3 (out)");
    any = true;
  }

  if (cgraph_get_shortest_path_dijkstra(g, &vpath, &epath, 0, 5, NULL, CGRAPH_OUT) != 0) {
    UT_MSG_FAILED("Return 0 if reached");
    any = true;
  }
  if (!cgraph_ivec_equal(vpath, (CGRAPH_INTEGER[]){0, 3, 5}, 3)) {
    cgraph_ivec_print(vpath);
    UT_MSG_FAILED("Case 7. No weight 0-5 (out)");
    any = true;
  }
  if (!cgraph_ivec_equal(epath, (CGRAPH_INTEGER[]){1, 6}, 2)) {
    cgraph_ivec_print(epath);
    UT_MSG_FAILED("Case 7. No weight 0-5 (out)");
    any = true;
  }

  /* crash test in the no path case*/
  cgraph_get_shortest_path_dijkstra(g, NULL, &epath, 3, 0, weights, CGRAPH_OUT);
  cgraph_get_shortest_path_dijkstra(g, &vpath, NULL, 3, 0, weights, CGRAPH_OUT);

  /* A little bit silly because it's a useless processing */
  cgraph_get_shortest_path_dijkstra(g, NULL, NULL, 3, 0, weights, CGRAPH_OUT);

  cgraph_destroy(&g);
  cgraph_ivec_free(&epath);
  cgraph_ivec_free(&vpath);
  cgraph_rvec_free(&weights);
  if (any) {
    return 1;
  }
  UT_MSG_OK("Test dijkstra");

  return 0;
}