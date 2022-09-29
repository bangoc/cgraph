/* (C) Nguyen Ba Ngoc 2022 */

/** @file
 * @brief Thử nghiệm sử dụng thư viện bkcgraph ở cấu hình hệ thống.
 */

#include <bkcgraph.h>
#include <bkcgen.h>

int main() {
  atype(CGRAPH_INTEGER) *edges = arr_ifrom_array(
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
  cgraph_print(g);
  arr_free(edges);
  cgraph_destroy(g);
  return 0;
}