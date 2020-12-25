#include "gb.h"
#include "ut.h"

/*
n = 6
directed = true
from  = {0, 0, 1, 1, 2, 0, 3, 3, 1}
to    = {1, 3, 2, 3, 4, 2, 5, 1, 4}
oi    = {0, 5, 1, 2, 3, 8, 4, 7, 6}
ii    = {0, 7, 5, 2, 1, 3, 8, 4, 6}
os    = {0, 3, 6, 7, 9, 9, 9}
is    = {0, 0, 2, 4, 6, 8, 9}
*/
int test_create_g1(cgraph_t *g) {
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
  return cgraph_create(g, edges, 0, true);
}

/*
n = 6
directed = true
from = {0, 0, 1, 1, 2, 0, 3, 3, 1, 2}
to   = {1, 3, 2, 3, 4, 2, 5, 1, 4, 2}
oi   = {0, 5, 1, 2, 3, 8, 9, 4, 7, 6}
ii   = {0, 7, 5, 2, 9, 1, 3, 8, 4, 6}
os   = {0, 3, 6, 8, 10, 10, 10}
is   = {0, 0, 2, 5, 7, 9, 10}
*/
int test_create_g2(cgraph_t *g) {
  cgraph_ivec_t edges = cgraph_ivec_fromarray(
        (CGRAPH_INTEGER[]){ 0, 1,
                            0, 3,
                            1, 2,
                            1, 3,
                            2, 4,
                            0, 2,
                            3, 5,
                            3, 1,
                            1, 4,
                            2, 2}, 20);
  return cgraph_create(g, edges, 0, true);
}

/*
n = 6
directed = true
from = {0, 0, 1, 1, 2, 0, 3, 1}
to   = {1, 3, 2, 3, 4, 2, 5, 4}
oi   = {0, 5, 1, 2, 3, 7, 4, 6}
ii   = {0, 5, 2, 1, 3, 7, 4, 6}
os   = {0, 3, 6, 7, 8, 8, 8}
is   = {0, 0, 1, 3, 5, 7, 8}
*/
int test_create_g3(cgraph_t *g) {
  cgraph_ivec_t edges = cgraph_ivec_fromarray(
        (CGRAPH_INTEGER[]){ 0, 1,
                            0, 3,
                            1, 2,
                            1, 3,
                            2, 4,
                            0, 2,
                            3, 5,
                            1, 4}, 16);
  return cgraph_create(g, edges, 0, true);
}

/*
n = 6
directed = false
from = {1, 3, 2, 3, 4, 2, 5, 4}
to   = {0, 0, 1, 1, 2, 0, 3, 1}
oi   = {0, 5, 2, 1, 3, 7, 4, 6}
ii   = {0, 5, 1, 2, 3, 7, 4, 6}
os   = {0, 0, 1, 3, 5, 7, 8}
is   = {0, 3, 6, 7, 8, 8, 8}
*/
int test_create_g4(cgraph_t *g) {
  cgraph_ivec_t edges = cgraph_ivec_fromarray(
        (CGRAPH_INTEGER[]){ 0, 1,
                            3, 0,
                            1, 2,
                            1, 3,
                            2, 4,
                            2, 0,
                            3, 5,
                            1, 4}, 16);
  return cgraph_create(g, edges, 0, false);
}

/*
n = 6
directed = false
from = {1, 3, 2, 3, 4, 2, 5, 4, 2}
to   = {0, 0, 1, 1, 2, 0, 3, 1, 2}
oi   = {0, 5, 2, 8, 1, 3, 7, 4, 6}
ii   = {0, 5, 1, 2, 3, 7, 8, 4, 6}
os   = {0, 0, 1, 4, 6, 8, 9}
is   = {0, 3, 6, 8, 9, 9, 9}
*/
int test_create_g5(cgraph_t *g) {
  cgraph_ivec_t edges = cgraph_ivec_fromarray(
        (CGRAPH_INTEGER[]){ 0, 1,
                            0, 3,
                            1, 2,
                            1, 3,
                            2, 4,
                            0, 2,
                            3, 5,
                            1, 4,
                            2, 2
                           }, 18);
  return cgraph_create(g, edges, 0, false);
}