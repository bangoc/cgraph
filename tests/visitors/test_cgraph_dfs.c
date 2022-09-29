#include <stdio.h>

#include "gb.h"
#include "ut.h"

int main() {
  cgraph_t g = test_create_g1();
  struct dfs *res = cgraph_dfs(g, 1, CGRAPH_OUT, false);
  bool any = false;
#define TEST(vec, arr, n, msg) { \
    if (!arr_iequal(vec, arr, n)) { \
      UT_MSG_FAILED(msg); \
      arr_iprint(vec); \
      any = true; \
    } \
  }
  /*
    order:      {1, 2, 4, 3, 5, NAN}
    order_out:  {4, 2, 5, 3, 1, NAN}
    father:     {NAN, -1, 1, 1, 2, 3}
    dist:       {NAN, 0, 1, 1, 2, 2}
  */
  TEST(res->order, ((CGRAPH_INTEGER[]){1, 2, 4, 3, 5, CGRAPH_NAN}), 6, "Case 1. order");
  TEST(res->order_out, ((CGRAPH_INTEGER[]){4, 2, 5, 3, 1, CGRAPH_NAN}), 6, "Case 1. order_out");
  TEST(res->father, ((CGRAPH_INTEGER[]){CGRAPH_NAN, -1, 1, 1, 2, 3}), 6, "Case 1. father");
  TEST(res->dist, ((CGRAPH_INTEGER[]){CGRAPH_NAN, 0, 1, 1, 2, 2}), 6, "Case 1. dist");
  dfs_free(res);

  res = cgraph_dfs(g, 1, CGRAPH_IN, false);
  /*
    order:    {1, 0, 3, NAN, NAN, NAN}
    order_out:  {0, 3, 1, NAN, NAN, NAN}
    father:   {1, -1, NAN, 1, NAN, NAN}
    dist:   {1, 0, NAN, 1, NAN, NAN}
  */
  TEST(res->order, ((CGRAPH_INTEGER[]){1, 0, 3, CGRAPH_NAN, CGRAPH_NAN, CGRAPH_NAN}), 6, "Case 2. order");
  TEST(res->order_out, ((CGRAPH_INTEGER[]){0, 3, 1, CGRAPH_NAN, CGRAPH_NAN, CGRAPH_NAN}), 6, "Case 2. order_out");
  TEST(res->father, ((CGRAPH_INTEGER[]){1, -1, CGRAPH_NAN, 1, CGRAPH_NAN, CGRAPH_NAN}), 6, "Case 2. father");
  TEST(res->dist, ((CGRAPH_INTEGER[]){1, 0, CGRAPH_NAN, 1, CGRAPH_NAN, CGRAPH_NAN}), 6, "Case 2. dist");
  dfs_free(res);

  res = cgraph_dfs(g, 1, CGRAPH_ALL, false);
  /*
    order:    {1, 0, 2, 4, 3, 5}
    order_out:  {4, 2, 5, 3, 0, 1}
    father:   {1, -1, 0, 0, 2, 3}
    dist:   {1, 0, 2, 2, 3, 3}
  */
  TEST(res->order, ((CGRAPH_INTEGER[]){1, 0, 2, 4, 3, 5}), 6, "Case 3. order");
  TEST(res->order_out, ((CGRAPH_INTEGER[]){4, 2, 5, 3, 0, 1}), 6, "Case 3. order_out");
  TEST(res->father, ((CGRAPH_INTEGER[]){1, -1, 0, 0, 2, 3}), 6, "Case 3. father");
  TEST(res->dist, ((CGRAPH_INTEGER[]){1, 0, 2, 2, 3, 3}), 6, "Case 3. dist");
  dfs_free(res);

  res = cgraph_dfs(g, 1, CGRAPH_OUT, true);
  /*
    order:    {1, 2, 4, 3, 5, 0}
    order_out:  {4, 2, 5, 3, 1, 0}
    father:   {-1, -1, 1, 1, 2, 3}
    dist:   {0, 0, 1, 1, 2, 2}
  */
  TEST(res->order, ((CGRAPH_INTEGER[]){1, 2, 4, 3, 5, 0}), 6, "Case 4. order");
  TEST(res->order_out, ((CGRAPH_INTEGER[]){4, 2, 5, 3, 1, 0}), 6, "Case 4. order_out");
  TEST(res->father, ((CGRAPH_INTEGER[]){-1, -1, 1, 1, 2, 3}), 6, "Case 4. father");
  TEST(res->dist, ((CGRAPH_INTEGER[]){0, 0, 1, 1, 2, 2}), 6, "Case 4. dist");
  dfs_free(res);

  cgraph_destroy(g);
  if (any) {
    return 1;
  }
  UT_MSG_OK("Test dfs");
  return 0;
}