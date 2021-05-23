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
cgraph_t test_create_g1() {
  cgraph_ivec_t edges = cgraph_ivec_from_array(
        (CGRAPH_INTEGER[]){ 0, 1,
                            0, 3,
                            1, 2,
                            1, 3,
                            2, 4,
                            0, 2,
                            3, 5,
                            3, 1,
                            1, 4}, 18);
  cgraph_t g = cgraph_create(edges, 0, true);
  cgraph_ivec_free(&edges);
  return g;
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
cgraph_t test_create_g2() {
  cgraph_ivec_t edges = cgraph_ivec_from_array(
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
  cgraph_t g = cgraph_create(edges, 0, true);
  cgraph_ivec_free(&edges);
  return g;
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
cgraph_t test_create_g3() {
  cgraph_ivec_t edges = cgraph_ivec_from_array(
        (CGRAPH_INTEGER[]){ 0, 1,
                            0, 3,
                            1, 2,
                            1, 3,
                            2, 4,
                            0, 2,
                            3, 5,
                            1, 4}, 16);
  cgraph_t g = cgraph_create(edges, 0, true);
  cgraph_ivec_free(&edges);
  return g;
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
cgraph_t test_create_g4() {
  cgraph_ivec_t edges = cgraph_ivec_from_array(
        (CGRAPH_INTEGER[]){ 0, 1,
                            3, 0,
                            1, 2,
                            1, 3,
                            2, 4,
                            2, 0,
                            3, 5,
                            1, 4}, 16);
  cgraph_t g = cgraph_create(edges, 0, false);
  cgraph_ivec_free(&edges);
  return g;
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
cgraph_t test_create_g5() {
  cgraph_ivec_t edges = cgraph_ivec_from_array(
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
  cgraph_t g = cgraph_create(edges, 0, false);
  cgraph_ivec_free(&edges);
  return g;
}

cgraph_t test_create_g6() {
  cgraph_ivec_t edges = cgraph_ivec_from_array(
    (CGRAPH_INTEGER[]) {0, 2,
                        0, 6,
                        0, 7,
                        2, 3,
                        3, 5,
                        3, 8,
                        4, 3,
                        4, 8,
                        5, 4,
                        5, 8,
                        6, 3,
                        6, 5,
                        6, 7,
                        7, 8
                    }, 28);
  cgraph_t g = cgraph_create(edges, 0, true);
  cgraph_ivec_free(&edges);
  return g;
}