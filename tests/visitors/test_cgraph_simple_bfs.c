#include <stdio.h>

#include "gb.h"
#include "ut.h"

int main() {
  cgraph_t g = test_create_g1();

  cgraph_ivec_t father = cgraph_ivec_create(),
                dist = cgraph_ivec_create();

  // printf("Case 1\n");
  cgraph_simple_bfs(g,
              /*root=*/1,
              /*neimode=*/ CGRAPH_OUT,
             /*unreachable=*/ 0,
             &father,
             &dist);

#define TEST(vec, arr, n, msg) \
  if (!cgraph_ivec_equal(vec, arr, n)) { \
    UT_MSG_FAILED(msg); \
    print_ivec(vec); \
    return 1; \
  }

  TEST(father, ((CGRAPH_INTEGER[]){CGRAPH_NAN, -1, 1, 1, 1, 3}), 6, "Case 1. father");
  TEST(dist, ((CGRAPH_INTEGER[]){CGRAPH_NAN, 0, 1, 1, 1, 2}), 6, "Case 1. dist");

  // printf("Case 2\n");
  cgraph_simple_bfs(g,
              /*root=*/1,
              /*neimode=*/ CGRAPH_IN,
             /*unreachable=*/ 0,
             &father,
             &dist);

  TEST(father, ((CGRAPH_INTEGER[]){1, -1, CGRAPH_NAN, 1, CGRAPH_NAN, CGRAPH_NAN}), 6, "Case 2. father");
  TEST(dist, ((CGRAPH_INTEGER[]){1, 0, CGRAPH_NAN, 1, CGRAPH_NAN, CGRAPH_NAN}), 6, "Case 2. dist");

  // printf("Case 3\n");
  cgraph_simple_bfs(g,
              /*root=*/1,
              /*neimode=*/ CGRAPH_ALL,
             /*unreachable=*/ 0,
             &father,
             &dist);

  TEST(father, ((CGRAPH_INTEGER[]){1, -1, 1, 1, 1, 3}), 6, "Case 3. father");
  TEST(dist, ((CGRAPH_INTEGER[]){1, 0, 1, 1, 1, 2}), 6, "Case 3. dist");

  // printf("Case 4\n");
  cgraph_simple_bfs(g,
              /*root=*/1,
              /*neimode=*/ CGRAPH_OUT,
             /*unreachable=*/ 1,
             &father,
             &dist);
  TEST(father, ((CGRAPH_INTEGER[]){-1, -1, 1, 1, 1, 3}), 6, "Case 4. father");
  TEST(dist, ((CGRAPH_INTEGER[]){0, 0, 1, 1, 1, 2}), 6, "Case 4. dist");
#undef TEST
  UT_MSG_OK("Test neightbors");
  return 0;
}