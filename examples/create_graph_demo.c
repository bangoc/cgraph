#include "cgraph.h"

int main() {
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
  cgraph_print(g);
  return 0;
}