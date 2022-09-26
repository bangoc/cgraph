#include "cgraph.h"

#include "cgen/tests/base/utils.h"
#include "tests/base/ut.h"

int main() {
  arr_ptr(CGRAPH_INTEGER) edges = arr_ifrom_array(
        (CGRAPH_INTEGER[]){ 0, 1,
                            0, 3,
                            1, 2}, 6);
  cgraph_t g = cgraph_create(edges, 0, true);
  CHECK_MSG(arr_iequal(g->from, (CGRAPH_INTEGER []){0, 0, 1}, 3), "g->from");
  CHECK_MSG(arr_iequal(g->to, (CGRAPH_INTEGER []){1, 3, 2}, 3), "g->to");
  CHECK_MSG(arr_iequal(g->oi, (CGRAPH_INTEGER []){0, 1, 2}, 3), "g->oi");
  CHECK_MSG(arr_iequal(g->ii, (CGRAPH_INTEGER []){0, 2, 1}, 3), "g->ii");
  CHECK_MSG(arr_iequal(g->os, (CGRAPH_INTEGER []){0, 2, 3, 3, 3}, 5), "g->os");
  CHECK_MSG(arr_iequal(g->is, (CGRAPH_INTEGER []){0, 0, 1, 2, 3}, 5), "g->is");
  CHECK_MSG(cgraph_vcount(g) == 4, "Số lượng đỉnh bằng 4");
  CHECK_MSG(cgraph_ecount(g) == 3, "Số lượng cạnh bằng 3");
  CHECK_MSG(cgraph_is_directed(g), "Đồ thị có hướng");

  //  Thêm một số cạnh
  arr_ptr(CGRAPH_INTEGER) edges2 = arr_ifrom_array(
          (CGRAPH_INTEGER[]){1, 3,
                             2, 0}, 4);
  cgraph_add_edges(g, edges2);
  CHECK_MSG(arr_iequal(g->from, (CGRAPH_INTEGER []){0, 0, 1, 1, 2}, 5), "g->from 2");
  CHECK_MSG(arr_iequal(g->to, (CGRAPH_INTEGER []){1, 3, 2, 3, 0}, 5), "g->to 2");
  CHECK_MSG(arr_iequal(g->oi, (CGRAPH_INTEGER []){0, 1, 2, 3, 4}, 5), "g->oi 2");
  CHECK_MSG(arr_iequal(g->ii, (CGRAPH_INTEGER []){4, 0, 2, 1, 3}, 5), "g->ii 2");
  CHECK_MSG(arr_iequal(g->os, (CGRAPH_INTEGER []){0, 2, 4, 5, 5}, 5), "g->os 2");
  CHECK_MSG(arr_iequal(g->is, (CGRAPH_INTEGER []){0, 1, 2, 3, 5}, 5), "g->is 2");
  CHECK_MSG(cgraph_vcount(g) == 4, "Số lượng đỉnh bằng 4");
  CHECK_MSG(cgraph_ecount(g) == 5, "Số lượng cạnh bằng 5");
  CHECK_MSG(cgraph_is_directed(g), "Đồ thị có hướng");

  // Thêm cạnh ngoài phạm vi
  arr_ptr(CGRAPH_INTEGER) edges3 = arr_ifrom_array(
          (CGRAPH_INTEGER[]) {5, 6}, 2);
  CHECK_MSG(cgraph_add_edges(g, edges3) == CGRAPH_FAILURE, "Trả về mã lỗi");

  // Đồ thị không thay đổi
  CHECK_MSG(arr_iequal(g->from, (CGRAPH_INTEGER []){0, 0, 1, 1, 2}, 5), "g->from 2");
  CHECK_MSG(arr_iequal(g->to, (CGRAPH_INTEGER []){1, 3, 2, 3, 0}, 5), "g->to 2");
  CHECK_MSG(arr_iequal(g->oi, (CGRAPH_INTEGER []){0, 1, 2, 3, 4}, 5), "g->oi 2");
  CHECK_MSG(arr_iequal(g->ii, (CGRAPH_INTEGER []){4, 0, 2, 1, 3}, 5), "g->ii 2");
  CHECK_MSG(arr_iequal(g->os, (CGRAPH_INTEGER []){0, 2, 4, 5, 5}, 5), "g->os 2");
  CHECK_MSG(arr_iequal(g->is, (CGRAPH_INTEGER []){0, 1, 2, 3, 5}, 5), "g->is 2");
  CHECK_MSG(cgraph_vcount(g) == 4, "Số lượng đỉnh bằng 4");
  CHECK_MSG(cgraph_ecount(g) == 5, "Số lượng cạnh bằng 5");
  CHECK_MSG(cgraph_is_directed(g), "Đồ thị có hướng");


  // Thêm đỉnh sau đó thêm cạnh
  cgraph_add_vertices(g, 3);
  CHECK_MSG(cgraph_vcount(g) == 7, "Số lượng đỉnh bằng 6");
  cgraph_add_edges(g, edges3);
  CHECK_MSG(arr_iequal(g->from, (CGRAPH_INTEGER []){0, 0, 1, 1, 2, 5}, 6), "g->from 3");
  CHECK_MSG(arr_iequal(g->to, (CGRAPH_INTEGER []){1, 3, 2, 3, 0, 6}, 6), "g->to 3");
  CHECK_MSG(arr_iequal(g->oi, (CGRAPH_INTEGER []){0, 1, 2, 3, 4, 5}, 6), "g->oi 3");
  CHECK_MSG(arr_iequal(g->ii, (CGRAPH_INTEGER []){4, 0, 2, 1, 3, 5}, 6), "g->ii 3");
  CHECK_MSG(arr_iequal(g->os, (CGRAPH_INTEGER []){0, 2, 4, 5, 5, 5, 6, 6}, 8), "g->os 3");
  CHECK_MSG(arr_iequal(g->is, (CGRAPH_INTEGER []){0, 1, 2, 3, 5, 5, 5, 6}, 8), "g->is 3");
  CHECK_MSG(cgraph_ecount(g) == 6, "Số lượng cạnh bằng 5");
  CHECK_MSG(cgraph_is_directed(g), "Đồ thị có hướng");

  arr_free(edges);
  arr_free(edges2);
  arr_free(edges3);
  cgraph_destroy(&g);
  return 0;
}