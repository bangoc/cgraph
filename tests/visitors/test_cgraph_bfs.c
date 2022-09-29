#include <stdio.h>

#include "gb.h"
#include "ut.h"

int main() {
  cgraph_t g = test_create_g1();

  // printf("Case 1\n");
  struct bfs *res = cgraph_bfs(g,
               /*root=*/1,
           /*neimode=*/ CGRAPH_OUT,
       /*unreachable=*/ 0,
        /*restricted=*/ NULL);

#define TEST(vec, arr, n, msg) \
  if (!arr_iequal(vec, arr, n)) { \
    UT_MSG_FAILED(msg); \
    arr_iprint(vec); \
    return 1; \
  }

  TEST(res->order, ((CGRAPH_INTEGER[]){1, 2, 3, 4, 5, CGRAPH_NAN}), 6, "Case 1. order");
  TEST(res->rank, ((CGRAPH_INTEGER[]){CGRAPH_NAN, 0, 1, 2, 3, 4}), 6, "Case 1. rank");
  TEST(res->father, ((CGRAPH_INTEGER[]){CGRAPH_NAN, -1, 1, 1, 1, 3}), 6, "Case 1. father");
  TEST(res->pred, ((CGRAPH_INTEGER[]){CGRAPH_NAN, -1, 1, 2, 3, 4}), 6, "Case 1. pred");
  TEST(res->succ, ((CGRAPH_INTEGER[]){CGRAPH_NAN, 2, 3, 4, 5, -1}), 6, "Case 1. succ");
  TEST(res->dist, ((CGRAPH_INTEGER[]){CGRAPH_NAN, 0, 1, 1, 1, 2}), 6, "Case 1. dist");
  bfs_free(res);

  // printf("Case 2\n");
  res = cgraph_bfs(g,
          /*root=*/1,
      /*neimode=*/ CGRAPH_IN,
  /*unreachable=*/ 0,
   /*restricted=*/ 0);
  TEST(res->order, ((CGRAPH_INTEGER[]){1, 0, 3, CGRAPH_NAN, CGRAPH_NAN, CGRAPH_NAN}), 6, "Case 2. order");
  TEST(res->rank, ((CGRAPH_INTEGER[]){1, 0, CGRAPH_NAN, 2, CGRAPH_NAN, CGRAPH_NAN}), 6, "Case 2. rank");
  TEST(res->father, ((CGRAPH_INTEGER[]){1, -1, CGRAPH_NAN, 1, CGRAPH_NAN, CGRAPH_NAN}), 6, "Case 2. father");
  TEST(res->pred, ((CGRAPH_INTEGER[]){1, -1, CGRAPH_NAN, 0, CGRAPH_NAN, CGRAPH_NAN}), 6, "Case 2. pred");
  TEST(res->succ, ((CGRAPH_INTEGER[]){3, 0, CGRAPH_NAN, -1, CGRAPH_NAN, CGRAPH_NAN}), 6, "Case 2. succ");
  TEST(res->dist, ((CGRAPH_INTEGER[]){1, 0, CGRAPH_NAN, 1, CGRAPH_NAN, CGRAPH_NAN}), 6, "Case 2. dist");
  bfs_free(res);

  // printf("Case 3\n");
  res = cgraph_bfs(g,
          /*root=*/1,
      /*neimode=*/ CGRAPH_ALL,
  /*unreachable=*/ 0,
   /*restricted=*/ 0);
  TEST(res->order, ((CGRAPH_INTEGER[]){1, 0, 2, 3, 4, 5}), 6, "Case 3. order");
  TEST(res->rank, ((CGRAPH_INTEGER[]){1, 0, 2, 3, 4, 5}), 6, "Case 3. rank");
  TEST(res->father, ((CGRAPH_INTEGER[]){1, -1, 1, 1, 1, 3}), 6, "Case 3. father");
  TEST(res->pred, ((CGRAPH_INTEGER[]){1, -1, 0, 2, 3, 4}), 6, "Case 3. pred");
  TEST(res->succ, ((CGRAPH_INTEGER[]){2, 0, 3, 4, 5, -1}), 6, "Case 3. succ");
  TEST(res->dist, ((CGRAPH_INTEGER[]){1, 0, 1, 1, 1, 2}), 6, "Case 3. dist");
  bfs_free(res);

  // printf("Case 4\n");
  res = cgraph_bfs(g,
          /*root=*/1,
      /*neimode=*/ CGRAPH_OUT,
  /*unreachable=*/ 1,
   /*restricted=*/ 0);
  TEST(res->order, ((CGRAPH_INTEGER[]){1, 2, 3, 4, 5, 0}), 6, "Case 4. order");
  TEST(res->rank, ((CGRAPH_INTEGER[]){5, 0, 1, 2, 3, 4}), 6, "Case 4. rank");
  TEST(res->father, ((CGRAPH_INTEGER[]){-1, -1, 1, 1, 1, 3}), 6, "Case 4. father");
  TEST(res->pred, ((CGRAPH_INTEGER[]){-1, -1, 1, 2, 3, 4}), 6, "Case 4. pred");
  TEST(res->succ, ((CGRAPH_INTEGER[]){-1, 2, 3, 4, 5, -1}), 6, "Case 4. succ");
  TEST(res->dist, ((CGRAPH_INTEGER[]){0, 0, 1, 1, 1, 2}), 6, "Case 4. dist");
  bfs_free(res);

#undef TEST
  UT_MSG_OK("Test neightbors");
  cgraph_destroy(g);
  return 0;
}