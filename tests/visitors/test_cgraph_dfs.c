#include <stdio.h>

#include "gb.h"
#include "ut.h"

int main() {
  cgraph_t g;
  test_create_g1(&g);
  cgraph_ivec_t order     = cgraph_ivec_create(),
                order_out = cgraph_ivec_create(),
                father    = cgraph_ivec_create(),
                dist      = cgraph_ivec_create();
  cgraph_dfs(&g, 1, CGRAPH_OUT, false, &order, &order_out, &father, &dist);
  bool any = false;
#define TEST(vec, arr, n, msg) { \
    if (!cgraph_ivec_equal(vec, arr, n)) { \
      UT_MSG_FAILED(msg); \
      print_ivec(vec); \
      any = true; \
    } \
  }
  /*
    order:      {1, 2, 4, 3, 5, NAN}
    order_out:  {4, 2, 5, 3, 1, NAN}
    father:     {NAN, -1, 1, 1, 2, 3}
    dist:       {NAN, 0, 1, 1, 2, 2}
  */
  TEST(order, ((CGRAPH_INTEGER[]){1, 2, 4, 3, 5, CGRAPH_NAN}), 6, "Case 1. order");
  TEST(order_out, ((CGRAPH_INTEGER[]){4, 2, 5, 3, 1, CGRAPH_NAN}), 6, "Case 1. order_out");
  TEST(father, ((CGRAPH_INTEGER[]){CGRAPH_NAN, -1, 1, 1, 2, 3}), 6, "Case 1. father");
  TEST(dist, ((CGRAPH_INTEGER[]){CGRAPH_NAN, 0, 1, 1, 2, 2}), 6, "Case 1. dist");
  
  cgraph_dfs(&g, 1, CGRAPH_IN, false, &order, &order_out, &father, &dist);
  /*
    order:    {1, 0, 3, NAN, NAN, NAN}
    order_out:  {0, 3, 1, NAN, NAN, NAN}
    father:   {1, -1, NAN, 1, NAN, NAN}
    dist:   {1, 0, NAN, 1, NAN, NAN}
  */
  TEST(order, ((CGRAPH_INTEGER[]){1, 0, 3, CGRAPH_NAN, CGRAPH_NAN, CGRAPH_NAN}), 6, "Case 2. order");
  TEST(order_out, ((CGRAPH_INTEGER[]){0, 3, 1, CGRAPH_NAN, CGRAPH_NAN, CGRAPH_NAN}), 6, "Case 2. order_out");
  TEST(father, ((CGRAPH_INTEGER[]){1, -1, CGRAPH_NAN, 1, CGRAPH_NAN, CGRAPH_NAN}), 6, "Case 2. father");
  TEST(dist, ((CGRAPH_INTEGER[]){1, 0, CGRAPH_NAN, 1, CGRAPH_NAN, CGRAPH_NAN}), 6, "Case 2. dist");

  cgraph_dfs(&g, 1, CGRAPH_ALL, false, &order, &order_out, &father, &dist);
  /*
    order:    {1, 0, 2, 4, 3, 5}
    order_out:  {4, 2, 5, 3, 0, 1}
    father:   {1, -1, 0, 0, 2, 3}
    dist:   {1, 0, 2, 2, 3, 3}
  */
  TEST(order, ((CGRAPH_INTEGER[]){1, 0, 2, 4, 3, 5}), 6, "Case 3. order");
  TEST(order_out, ((CGRAPH_INTEGER[]){4, 2, 5, 3, 0, 1}), 6, "Case 3. order_out");
  TEST(father, ((CGRAPH_INTEGER[]){1, -1, 0, 0, 2, 3}), 6, "Case 3. father");
  TEST(dist, ((CGRAPH_INTEGER[]){1, 0, 2, 2, 3, 3}), 6, "Case 3. dist");
  
  cgraph_dfs(&g, 1, CGRAPH_OUT, true, &order, &order_out, &father, &dist);
  /*
    order:    {1, 2, 4, 3, 5, 0}
    order_out:  {4, 2, 5, 3, 1, 0}
    father:   {-1, -1, 1, 1, 2, 3}
    dist:   {0, 0, 1, 1, 2, 2}
  */
  TEST(order, ((CGRAPH_INTEGER[]){1, 2, 4, 3, 5, 0}), 6, "Case 4. order");
  TEST(order_out, ((CGRAPH_INTEGER[]){4, 2, 5, 3, 1, 0}), 6, "Case 4. order_out");
  TEST(father, ((CGRAPH_INTEGER[]){-1, -1, 1, 1, 2, 3}), 6, "Case 4. father");
  TEST(dist, ((CGRAPH_INTEGER[]){0, 0, 1, 1, 2, 2}), 6, "Case 4. dist");
  cgraph_destroy(&g);
  if (any) {
    return 1;
  }
  UT_MSG_OK("Test dfs");
  return 0;
}