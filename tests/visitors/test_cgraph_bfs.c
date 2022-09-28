#include <stdio.h>

#include "gb.h"
#include "ut.h"

int main() {
  cgraph_t g = test_create_g1();

  arr_make(order, 0, CGRAPH_INTEGER);
  arr_make(rank, 0, CGRAPH_INTEGER);
  arr_make(father, 0, CGRAPH_INTEGER);
  arr_make(pred, 0, CGRAPH_INTEGER);
  arr_make(succ, 0, CGRAPH_INTEGER);
  arr_make(dist, 0, CGRAPH_INTEGER);

  // printf("Case 1\n");
  cgraph_bfs(g,
              /*root=*/1,
              /*neimode=*/ CGRAPH_OUT,
             /*unreachable=*/ 0,
             /*restricted=*/ 0,
             &order,
             &rank,
             &father,
             &pred,
             &succ,
             &dist);

#define TEST(vec, arr, n, msg) \
  if (!arr_iequal(vec, arr, n)) { \
    UT_MSG_FAILED(msg); \
    arr_iprint(vec); \
    return 1; \
  }

  TEST(order, ((CGRAPH_INTEGER[]){1, 2, 3, 4, 5, CGRAPH_NAN}), 6, "Case 1. order");
  TEST(rank, ((CGRAPH_INTEGER[]){CGRAPH_NAN, 0, 1, 2, 3, 4}), 6, "Case 1. rank");
  TEST(father, ((CGRAPH_INTEGER[]){CGRAPH_NAN, -1, 1, 1, 1, 3}), 6, "Case 1. father");
  TEST(pred, ((CGRAPH_INTEGER[]){CGRAPH_NAN, -1, 1, 2, 3, 4}), 6, "Case 1. pred");
  TEST(succ, ((CGRAPH_INTEGER[]){CGRAPH_NAN, 2, 3, 4, 5, -1}), 6, "Case 1. succ");
  TEST(dist, ((CGRAPH_INTEGER[]){CGRAPH_NAN, 0, 1, 1, 1, 2}), 6, "Case 1. dist");

  // printf("Case 2\n");
  cgraph_bfs(g,
              /*root=*/1,
              /*neimode=*/ CGRAPH_IN,
             /*unreachable=*/ 0,
             /*restricted=*/ 0,
             &order,
             &rank,
             &father,
             &pred,
             &succ,
             &dist);

  TEST(order, ((CGRAPH_INTEGER[]){1, 0, 3, CGRAPH_NAN, CGRAPH_NAN, CGRAPH_NAN}), 6, "Case 2. order");
  TEST(rank, ((CGRAPH_INTEGER[]){1, 0, CGRAPH_NAN, 2, CGRAPH_NAN, CGRAPH_NAN}), 6, "Case 2. rank");
  TEST(father, ((CGRAPH_INTEGER[]){1, -1, CGRAPH_NAN, 1, CGRAPH_NAN, CGRAPH_NAN}), 6, "Case 2. father");
  TEST(pred, ((CGRAPH_INTEGER[]){1, -1, CGRAPH_NAN, 0, CGRAPH_NAN, CGRAPH_NAN}), 6, "Case 2. pred");
  TEST(succ, ((CGRAPH_INTEGER[]){3, 0, CGRAPH_NAN, -1, CGRAPH_NAN, CGRAPH_NAN}), 6, "Case 2. succ");
  TEST(dist, ((CGRAPH_INTEGER[]){1, 0, CGRAPH_NAN, 1, CGRAPH_NAN, CGRAPH_NAN}), 6, "Case 2. dist");

  // printf("Case 3\n");
  cgraph_bfs(g,
              /*root=*/1,
              /*neimode=*/ CGRAPH_ALL,
             /*unreachable=*/ 0,
             /*restricted=*/ 0,
             &order,
             &rank,
             &father,
             &pred,
             &succ,
             &dist);

  TEST(order, ((CGRAPH_INTEGER[]){1, 0, 2, 3, 4, 5}), 6, "Case 3. order");
  TEST(rank, ((CGRAPH_INTEGER[]){1, 0, 2, 3, 4, 5}), 6, "Case 3. rank");
  TEST(father, ((CGRAPH_INTEGER[]){1, -1, 1, 1, 1, 3}), 6, "Case 3. father");
  TEST(pred, ((CGRAPH_INTEGER[]){1, -1, 0, 2, 3, 4}), 6, "Case 3. pred");
  TEST(succ, ((CGRAPH_INTEGER[]){2, 0, 3, 4, 5, -1}), 6, "Case 3. succ");
  TEST(dist, ((CGRAPH_INTEGER[]){1, 0, 1, 1, 1, 2}), 6, "Case 3. dist");

  // printf("Case 4\n");
  cgraph_bfs(g,
              /*root=*/1,
              /*neimode=*/ CGRAPH_OUT,
             /*unreachable=*/ 1,
             /*restricted=*/ 0,
             &order,
             &rank,
             &father,
             &pred,
             &succ,
             &dist);
  TEST(order, ((CGRAPH_INTEGER[]){1, 2, 3, 4, 5, 0}), 6, "Case 4. order");
  TEST(rank, ((CGRAPH_INTEGER[]){5, 0, 1, 2, 3, 4}), 6, "Case 4. rank");
  TEST(father, ((CGRAPH_INTEGER[]){-1, -1, 1, 1, 1, 3}), 6, "Case 4. father");
  TEST(pred, ((CGRAPH_INTEGER[]){-1, -1, 1, 2, 3, 4}), 6, "Case 4. pred");
  TEST(succ, ((CGRAPH_INTEGER[]){-1, 2, 3, 4, 5, -1}), 6, "Case 4. succ");
  TEST(dist, ((CGRAPH_INTEGER[]){0, 0, 1, 1, 1, 2}), 6, "Case 4. dist");
#undef TEST
  UT_MSG_OK("Test neightbors");
  cgraph_destroy(g);
  arr_free(order);
  arr_free(rank);
  arr_free(father);
  arr_free(pred);
  arr_free(succ);
  arr_free(dist);
  return 0;
}