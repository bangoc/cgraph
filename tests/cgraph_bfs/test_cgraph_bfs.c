#include <stdio.h>

#include "cgraph.h"
#include "ut.h"

int main() {
  cgraph_t g;
  cgraph_ivec_t edges = cgraph_ivec_fromarray(
        (CGRAPH_INTEGER[]){ 0, 1, 
                            0, 3,
                            1, 2,
                            1, 3,
                            2, 4,
                            0, 2,
                            3, 5,
                            3, 1,
                            1, 4}, 18);
  cgraph_create(&g, edges, 0, true);
  
  cgraph_ivec_t order = cgraph_ivec_create(), 
                rank = cgraph_ivec_create(), 
                father = cgraph_ivec_create(), 
                pred = cgraph_ivec_create(), 
                succ = cgraph_ivec_create(), 
                dist = cgraph_ivec_create();
  
  // printf("Case 1\n");
  cgraph_bfs(&g, 
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

  if (!cgraph_ivec_equal(order, (CGRAPH_INTEGER[]){1, 2, 3, 4, 5, CGRAPH_NAN}, 6)) {
    UT_MSG_FAILED("Case 1. order");
    print_ivec(order);
    return 1;
  }

  if (!cgraph_ivec_equal(rank, (CGRAPH_INTEGER[]){CGRAPH_NAN, 0, 1, 2, 3, 4}, 6)) {
    UT_MSG_FAILED("Case 1. rank");
    print_ivec(rank);
    return 1;
  }

  if (!cgraph_ivec_equal(father, (CGRAPH_INTEGER[]){CGRAPH_NAN, -1, 1, 1, 1, 3}, 6)) {
    UT_MSG_FAILED("Case 1. father");
    print_ivec(father);
    return 1;
  }

  if (!cgraph_ivec_equal(pred, (CGRAPH_INTEGER[]){CGRAPH_NAN, -1, 1, 2, 3, 4}, 6)) {
    UT_MSG_FAILED("Case 1. pred");
    print_ivec(pred);
    return 1;
  }

  if (!cgraph_ivec_equal(succ, (CGRAPH_INTEGER[]){CGRAPH_NAN, 2, 3, 4, 5, -1}, 6)) {
    UT_MSG_FAILED("Case 1. succ");
    print_ivec(succ);
    return 1;
  }

  if (!cgraph_ivec_equal(dist, (CGRAPH_INTEGER[]){CGRAPH_NAN, 0, 1, 1, 1, 2}, 6)) {
    UT_MSG_FAILED("Case 1. dist");
    print_ivec(dist);
    return 1;
  }

  // printf("Case 2\n");
  cgraph_bfs(&g, 
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

  if (!cgraph_ivec_equal(order, (CGRAPH_INTEGER[]){1, 0, 3, CGRAPH_NAN, CGRAPH_NAN, CGRAPH_NAN}, 6)) {
    UT_MSG_FAILED("Case 2. order");
    return 1;
  }

  if (!cgraph_ivec_equal(rank, (CGRAPH_INTEGER[]){1, 0, CGRAPH_NAN, 2, CGRAPH_NAN, CGRAPH_NAN}, 6)) {
    UT_MSG_FAILED("Case 2. rank");
    return 1;
  }

  if (!cgraph_ivec_equal(father, (CGRAPH_INTEGER[]){1, -1, CGRAPH_NAN, 1, CGRAPH_NAN, CGRAPH_NAN}, 6)) {
    UT_MSG_FAILED("Case 2. father");
    return 1;
  }

  if (!cgraph_ivec_equal(pred, (CGRAPH_INTEGER[]){1, -1, CGRAPH_NAN, 0, CGRAPH_NAN, CGRAPH_NAN}, 6)) {
    UT_MSG_FAILED("Case 2. pred");
    return 1;
  }

  if (!cgraph_ivec_equal(succ, (CGRAPH_INTEGER[]){3, 0, CGRAPH_NAN, -1, CGRAPH_NAN, CGRAPH_NAN}, 6)) {
    UT_MSG_FAILED("Case 2. succ");
    return 1;
  }

  if (!cgraph_ivec_equal(dist, (CGRAPH_INTEGER[]){1, 0, CGRAPH_NAN, 1, CGRAPH_NAN, CGRAPH_NAN}, 6)) {
    UT_MSG_FAILED("Case 2. dist");
    return 1;
  }
  
  // printf("Case 3\n");
  cgraph_bfs(&g, 
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

  if (!cgraph_ivec_equal(order, (CGRAPH_INTEGER[]){1, 0, 2, 3, 4, 5}, 6)) {
    UT_MSG_FAILED("Case 3. order");
    return 1;
  }

  if (!cgraph_ivec_equal(rank, (CGRAPH_INTEGER[]){1, 0, 2, 3, 4, 5}, 6)) {
    UT_MSG_FAILED("Case 3. rank");
    return 1;
  }

  if (!cgraph_ivec_equal(father, (CGRAPH_INTEGER[]){1, -1, 1, 1, 1, 3}, 6)) {
    UT_MSG_FAILED("Case 3. father");
    return 1;
  }

  if (!cgraph_ivec_equal(pred, (CGRAPH_INTEGER[]){1, -1, 0, 2, 3, 4}, 6)) {
    UT_MSG_FAILED("Case 3. pred");
    return 1;
  }

  if (!cgraph_ivec_equal(succ, (CGRAPH_INTEGER[]){2, 0, 3, 4, 5, -1}, 6)) {
    UT_MSG_FAILED("Case 3. succ");
    return 1;
  }

  if (!cgraph_ivec_equal(dist, (CGRAPH_INTEGER[]){1, 0, 1, 1, 1, 2}, 6)) {
    UT_MSG_FAILED("Case 3. dist");
    return 1;
  }
  UT_MSG_OK("Test neightbors");
  return 0;
}