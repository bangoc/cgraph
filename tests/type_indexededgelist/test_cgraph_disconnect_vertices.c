#include "cgraph.h"

#include "cgen/tests/base/utils.h"
#include "tests/base/ut.h"

int main() {
  cgraph_ivec_t edges =
      cgraph_ivec_from_array((CGRAPH_INTEGER[]){0, 1,
                                                2, 3,
                                                5, 6,
                                                0, 4,
                                                1, 3,
                                                2, 6,
                                                5, 3,
                                                1, 2,
                                                3, 6}, 18);
  cgraph_t g = cgraph_create(edges, 0, CGRAPH_DIRECTED);
  CHECK_MSG(cgraph_ecount(g) == 9, "Số lượng cạnh ban đầu bằng 9");
  CHECK_MSG(cgraph_vcount(g) == 7, "Số lượng đỉnh ban đầu bằng 7");
  CHECK_MSG(cgraph_ivec_equal(g->from,
      (CGRAPH_INTEGER[]){0, 2, 5, 0, 1, 2, 5, 1, 3}, 9), "g->from");
  CHECK_MSG(cgraph_ivec_equal(g->to,
      (CGRAPH_INTEGER[]){1, 3, 6, 4, 3, 6, 3, 2, 6}, 9), "g->to");
  CHECK_MSG(cgraph_ivec_equal(g->oi,
      (CGRAPH_INTEGER[]){0, 3, 7, 4, 1, 5, 8, 6, 2}, 9), "g->oi");
  CHECK_MSG(cgraph_ivec_equal(g->ii,
      (CGRAPH_INTEGER[]){0, 7, 4, 1, 6, 3, 5, 8, 2}, 9), "g->ii");
  CHECK_MSG(cgraph_ivec_equal(g->os,
      (CGRAPH_INTEGER[]){0, 2, 4, 6, 7, 7, 9, 9}, 8), "g->os");
  CHECK_MSG(cgraph_ivec_equal(g->is,
      (CGRAPH_INTEGER[]){0, 0, 1, 2, 5, 6, 6, 9}, 8), "g->is");
  cgraph_ivec_t v = cgraph_ivec_create();
  cgraph_ivec_push_back(&v, 1);

  // Xóa các cạnh đi tới 1
  cgraph_disconnect_vertices(g, v, CGRAPH_IN);

  CHECK_MSG(cgraph_ecount(g) == 8, "Số lượng cạnh còn lại bằng 8");
  CHECK_MSG(cgraph_vcount(g) == 7, "Số lượng đỉnh không thay đổi bằng 7");
  CHECK_MSG(cgraph_ivec_equal(g->from,
      (CGRAPH_INTEGER[]){2, 5, 0, 1, 2, 5, 1, 3}, 8), "g->from 1");
  CHECK_MSG(cgraph_ivec_equal(g->to,
      (CGRAPH_INTEGER[]){3, 6, 4, 3, 6, 3, 2, 6}, 8), "g->to 1");
  CHECK_MSG(cgraph_ivec_equal(g->oi,
      (CGRAPH_INTEGER[]){2, 6, 3, 0, 4, 7, 5, 1}, 8), "g->oi 1");
  CHECK_MSG(cgraph_ivec_equal(g->ii,
      (CGRAPH_INTEGER[]){6, 3, 0, 5, 2, 4, 7, 1}, 8), "g->ii 1");
  CHECK_MSG(cgraph_ivec_equal(g->os,
      (CGRAPH_INTEGER[]){0, 1, 3, 5, 6, 6, 8, 8}, 8), "g->os 1");
  CHECK_MSG(cgraph_ivec_equal(g->is,
      (CGRAPH_INTEGER[]){0, 0, 0, 1, 4, 5, 5, 8}, 8), "g->is 1");

  // Xóa các cạnh đi ra khỏi 1
  cgraph_disconnect_vertices(g, v, CGRAPH_OUT);

  CHECK_MSG(cgraph_ecount(g) == 6, "Số lượng cạnh còn lại bằng 6");
  CHECK_MSG(cgraph_vcount(g) == 7, "Số lượng đỉnh không thay đổi bằng 7");
  CHECK_MSG(cgraph_ivec_equal(g->from,
      (CGRAPH_INTEGER[]){2, 5, 0, 2, 5, 3}, 6), "g->from 2");
  CHECK_MSG(cgraph_ivec_equal(g->to,
      (CGRAPH_INTEGER[]){3, 6, 4, 6, 3, 6}, 6), "g->to 2");
  CHECK_MSG(cgraph_ivec_equal(g->oi,
      (CGRAPH_INTEGER[]){2, 0, 3, 5, 4, 1}, 6), "g->oi 2");
  CHECK_MSG(cgraph_ivec_equal(g->ii,
      (CGRAPH_INTEGER[]){0, 4, 2, 3, 5, 1}, 6), "g->ii 2");
  CHECK_MSG(cgraph_ivec_equal(g->os,
      (CGRAPH_INTEGER[]){0, 1, 1, 3, 4, 4, 6, 6}, 8), "g->os 2");
  CHECK_MSG(cgraph_ivec_equal(g->is,
      (CGRAPH_INTEGER[]){0, 0, 0, 0, 2, 3, 3, 6}, 8), "g->is 2");

  cgraph_ivec_setsize(v, 0);
  cgraph_ivec_push_back(&v, 0);
  cgraph_ivec_push_back(&v, 0);  // cho phép trùng lặp
  cgraph_ivec_push_back(&v, 3);
  cgraph_ivec_push_back(&v, 5);
  cgraph_disconnect_vertices(g, v, CGRAPH_ALL);

  CHECK_MSG(cgraph_ecount(g) == 1, "Số lượng cạnh còn lại bằng 1");
  CHECK_MSG(cgraph_vcount(g) == 7, "Số lượng đỉnh không thay đổi bằng 7");
  CHECK_MSG(cgraph_ivec_equal(g->from,
      (CGRAPH_INTEGER[]){2}, 1), "g->from 3");
  CHECK_MSG(cgraph_ivec_equal(g->to,
      (CGRAPH_INTEGER[]){6}, 1), "g->to 3");
  CHECK_MSG(cgraph_ivec_equal(g->oi,
      (CGRAPH_INTEGER[]){0}, 1), "g->oi 3");
  CHECK_MSG(cgraph_ivec_equal(g->ii,
      (CGRAPH_INTEGER[]){0}, 1), "g->ii 3");
  CHECK_MSG(cgraph_ivec_equal(g->os,
      (CGRAPH_INTEGER[]){0, 0, 0, 1, 1, 1, 1, 1}, 8), "g->os 3");
  CHECK_MSG(cgraph_ivec_equal(g->is,
      (CGRAPH_INTEGER[]){0, 0, 0, 0, 0, 0, 0, 1}, 8), "g->is 3");

  cgraph_ivec_setsize(v, 0);
  cgraph_ivec_push_back(&v, 2);
  cgraph_ivec_push_back(&v, 7);  // Đỉnh không hợp lệ
  CHECK_MSG(cgraph_disconnect_vertices(g, v, CGRAPH_ALL) == CGRAPH_FAILURE,
              "Lỗi ngắt kết nối đỉnh không hợp lệ");
  // Cấu trúc đồ thị được giữ nguyên không thay đổi
  CHECK_MSG(cgraph_ecount(g) == 1, "Số lượng cạnh còn lại bằng 1");
  CHECK_MSG(cgraph_vcount(g) == 7, "Số lượng đỉnh không thay đổi bằng 7");
  CHECK_MSG(cgraph_ivec_equal(g->from,
      (CGRAPH_INTEGER[]){2}, 1), "g->from 4");
  CHECK_MSG(cgraph_ivec_equal(g->to,
      (CGRAPH_INTEGER[]){6}, 1), "g->to 4");
  CHECK_MSG(cgraph_ivec_equal(g->oi,
      (CGRAPH_INTEGER[]){0}, 1), "g->oi 4");
  CHECK_MSG(cgraph_ivec_equal(g->ii,
      (CGRAPH_INTEGER[]){0}, 1), "g->ii 4");
  CHECK_MSG(cgraph_ivec_equal(g->os,
      (CGRAPH_INTEGER[]){0, 0, 0, 1, 1, 1, 1, 1}, 8), "g->os 4");
  CHECK_MSG(cgraph_ivec_equal(g->is,
      (CGRAPH_INTEGER[]){0, 0, 0, 0, 0, 0, 0, 1}, 8), "g->is 4");


  cgraph_ivec_setsize(v, 0);
  cgraph_ivec_push_back(&v, 3);
  cgraph_ivec_push_back(&v, 5);
  cgraph_ivec_push_back(&v, 0);
  cgraph_disconnect_vertices(g, v, CGRAPH_ALL);

  // Không có cạnh nào, cấu trúc đồ thị được giữ nguyên không thay đổi
  CHECK_MSG(cgraph_ecount(g) == 1, "Số lượng cạnh còn lại bằng 1");
  CHECK_MSG(cgraph_vcount(g) == 7, "Số lượng đỉnh không thay đổi bằng 7");
  CHECK_MSG(cgraph_ivec_equal(g->from,
      (CGRAPH_INTEGER[]){2}, 1), "g->from 5");
  CHECK_MSG(cgraph_ivec_equal(g->to,
      (CGRAPH_INTEGER[]){6}, 1), "g->to 5");
  CHECK_MSG(cgraph_ivec_equal(g->oi,
      (CGRAPH_INTEGER[]){0}, 1), "g->oi 5");
  CHECK_MSG(cgraph_ivec_equal(g->ii,
      (CGRAPH_INTEGER[]){0}, 1), "g->ii 5");
  CHECK_MSG(cgraph_ivec_equal(g->os,
      (CGRAPH_INTEGER[]){0, 0, 0, 1, 1, 1, 1, 1}, 8), "g->os 5");
  CHECK_MSG(cgraph_ivec_equal(g->is,
      (CGRAPH_INTEGER[]){0, 0, 0, 0, 0, 0, 0, 1}, 8), "g->is 5");

  cgraph_ivec_free(&v);
  cgraph_ivec_free(&edges);
  cgraph_destroy(&g);
  return 0;
}
