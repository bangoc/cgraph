#include "cgraph.h"
#include "cgen/tests/base/utils.h"
#include "tests/base/ut.h"

int main() {
  cgraph_t g = cgraph_create_empty(0, true);
  CHECK_MSG(cgraph_vcount(g) == 0, "Số đỉnh của đồ thị rỗng bằng 0");
  cgraph_add_vertices(g, 10);
  CHECK_MSG(cgraph_vcount(g) == 10, "Sau khi thêm 10 đỉnh");
  cgraph_add_vertices(g, -10);
  CHECK_MSG(cgraph_err_get() == CGRAPH_FAILURE, "Lỗi thêm số đỉnh âm");

  CHECK_MSG(cgraph_vcount(g) == 10, "Số đỉnh giữ nguyên sau khi thêm số đỉnh âm");

  cgraph_add_vertices(g, 20);
  CHECK_MSG(cgraph_vcount(g) == 30, "Sau khi thêm tiếp 20 đỉnh");
  cgraph_destroy(g);
  return 0;
}