#include "cgraph.h"

#include "cgen/tests/base/utils.h"
#include "tests/base/ut.h"

int main() {
  arr_ptr(CGRAPH_INTEGER) edges = arr_ifrom_array(
         (CGRAPH_INTEGER[]){0, 1,
                            2, 3,
                            1, 3,
                            0, 2,
                            1, 2,
                            0, 3,
                            2, 5,
                            5, 3}, 16);
  cgraph_t g = cgraph_create(edges, 0, CGRAPH_DIRECTED);
  CHECK_MSG(cgraph_is_directed(g), "Đồ thị có hướng");
  CHECK_MSG(cgraph_vcount(g) == 6, "Số lượng đỉnh bằng 6");
  CHECK_MSG(cgraph_ecount(g) == 8, "Số lượng cạnh bằng 8");
  CHECK_MSG(arr_iequal(g->from, (CGRAPH_INTEGER []){0, 2, 1, 0, 1, 0, 2, 5}, 8), "g->from");
  CHECK_MSG(arr_iequal(g->to, (CGRAPH_INTEGER [])  {1, 3, 3, 2, 2, 3, 5, 3}, 8), "g->to");
  CHECK_MSG(arr_iequal(g->oi, (CGRAPH_INTEGER [])  {0, 3, 5, 4, 2, 1, 6, 7}, 8), "g->oi");
  CHECK_MSG(arr_iequal(g->ii, (CGRAPH_INTEGER [])  {0, 3, 4, 5, 2, 1, 7, 6}, 8), "g->ii");
  CHECK_MSG(arr_iequal(g->os, (CGRAPH_INTEGER []){0, 3, 5, 7, 7, 7, 8}, 7), "g->os");
  CHECK_MSG(arr_iequal(g->is, (CGRAPH_INTEGER []){0, 0, 1, 3, 7, 7, 8}, 7), "g->is");

  arr_ptr(CGRAPH_INTEGER) v1 = arr_ifrom_array(
    (CGRAPH_INTEGER[]){0, 2, 4}, 3);
  cgraph_delete_edges(g, v1);
  CHECK_MSG(cgraph_is_directed(g), "Đồ thị có hướng");
  CHECK_MSG(cgraph_vcount(g) == 6, "Số lượng đỉnh bằng 6");
  CHECK_MSG(cgraph_ecount(g) == 5, "Số lượng cạnh bằng 5");

  // Đồ thị sau khi xóa các cạnh số 0, 2, 4
  CHECK_MSG(arr_iequal(g->from, (CGRAPH_INTEGER []){2, 0, 0, 2, 5}, 5), "g->from 1");
  CHECK_MSG(arr_iequal(g->to, (CGRAPH_INTEGER [])  {3, 2, 3, 5, 3}, 5), "g->to 1");
  CHECK_MSG(arr_iequal(g->oi, (CGRAPH_INTEGER [])  {1, 2, 0, 3, 4}, 5), "g->oi 1");
  CHECK_MSG(arr_iequal(g->ii, (CGRAPH_INTEGER [])  {1, 2, 0, 4, 3}, 5), "g->ii 1");
  CHECK_MSG(arr_iequal(g->os, (CGRAPH_INTEGER [])  {0, 2, 2, 4, 4, 4, 5}, 7), "g->os 1");
  CHECK_MSG(arr_iequal(g->is, (CGRAPH_INTEGER [])  {0, 0, 0, 1, 4, 4, 5}, 7), "g->is 1");

  arr_make(v2, 0, CGRAPH_INTEGER);
  arr_append(v2, 1);
  arr_append(v2, 3);
  cgraph_delete_edges(g, v2);
  CHECK_MSG(cgraph_is_directed(g), "Đồ thị có hướng");
  CHECK_MSG(cgraph_vcount(g) == 6, "Số lượng đỉnh bằng 6");
  CHECK_MSG(cgraph_ecount(g) == 3, "Số lượng cạnh bằng 3");

  // Đồ thị sau khi xóa các cạnh số 1, 3
  CHECK_MSG(arr_iequal(g->from, (CGRAPH_INTEGER []){2, 0, 5}, 3), "g->from");
  CHECK_MSG(arr_iequal(g->to, (CGRAPH_INTEGER [])  {3, 3, 3}, 3), "g->to");
  CHECK_MSG(arr_iequal(g->oi, (CGRAPH_INTEGER [])  {1, 0, 2}, 3), "g->oi");
  CHECK_MSG(arr_iequal(g->ii, (CGRAPH_INTEGER [])  {1, 0, 2}, 3), "g->ii");
  CHECK_MSG(arr_iequal(g->os, (CGRAPH_INTEGER [])  {0, 1, 1, 2, 2, 2, 3}, 7), "g->os");
  CHECK_MSG(arr_iequal(g->is, (CGRAPH_INTEGER [])  {0, 0, 0, 0, 3, 3, 3}, 7), "g->is");
  arr_free(edges);
  arr_free(v1);
  arr_free(v2);
  cgraph_destroy(&g);
  return 0;
}