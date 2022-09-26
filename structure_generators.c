#include "cgraph_constructors.h"
#include "cgraph_error.h"
#include "cgraph_interface.h"

#include <stdlib.h>
#include <stdio.h>

/**
 * \ingroup Giao diện
 * \function cgraph_create_empty
 * \brief Tạo một đồ thị rỗng với số lượng đỉnh được cho nhưng không
 * có cạnh.
 *
 * </para><para>
 * Đây là hàm tạo cơ bản, các hàm tạo khác cần gọi hàm tạo này để tạo
 * đối tượng đồ thị tối thiểu.
 * \param n Số lượng đỉnh của đồ thị, phải là một số nguyên không âm.
 * \param directed Kiểu bool xác định tính chất có hướng của đồ thị.
 * Các giá trị được hỗ trợ là:
 *    \clist
 *    \cli IGRAPH_DIRECTED
 *      Đồ thị \em có hướng.
 *    \cli CGRAPH_UNDIRECTED
 *      Đồ thị \em vô hướng.
 *    \endclist
 * \return NULL nếu không thể tạo đồ thị, hoặc con trỏ đồ thị mới
 * được tạo.
 *
 * Độ phức tạp: O(|V|) đối với một đồ thị với
 * |V| đỉnh (và không có cạnh).
 *
 */

cgraph_t cgraph_create_empty(CGRAPH_INTEGER n, bool directed) {

  if (n < 0) {
    // Không thể tạo đồ thị rỗng với số lượng đỉnh âm
    return NULL;
  }

  cgraph_t graph = malloc(sizeof(struct cgraph_s));

  graph->n = 0;
  graph->directed = directed;
  graph->from = arr_create(0, CGRAPH_INTEGER);
  graph->to = arr_create(0, CGRAPH_INTEGER);
  graph->oi = arr_create(0, CGRAPH_INTEGER);
  graph->ii = arr_create(0, CGRAPH_INTEGER);
  graph->os = arr_create(0, CGRAPH_INTEGER);
  graph->is = arr_create(0, CGRAPH_INTEGER);

  arr_append(graph->os, 0);
  arr_append(graph->is, 0);

  /* thêm các đỉnh */
  if (cgraph_add_vertices(graph, n) != CGRAPH_SUCCESS) {
    return NULL;
  }

  return graph;
}

cgraph_t cgraph_create(arr_ptr(CGRAPH_INTEGER) edges,
          CGRAPH_INTEGER n,
          bool directed) {
  bool has_edges = arr_size(edges) > 0;
  CGRAPH_INTEGER max = has_edges ? arr_imax(edges) + 1 : 0;

  if (arr_size(edges) % 2 != 0) {
    CGRAPH_WARNING("Vec-tơ cạnh không hợp lệ (độ dài lẻ)");
    return NULL;
  }
  if (has_edges && !arr_irange(edges, 0, max - 1)) {
    CGRAPH_WARNING("Chỉ số đỉnh không hợp lệ (giá trị nằm ngoài khoảng)");
    return NULL;
  }

  cgraph_t graph = cgraph_create_empty(n, directed);

  if (has_edges) {
    CGRAPH_INTEGER vc = cgraph_vcount(graph);
    if (vc < max) {
      if (cgraph_add_vertices(graph, (CGRAPH_INTEGER) (max - vc)) != CGRAPH_SUCCESS) {
        CGRAPH_WARNING("Lỗi thêm đỉnh.");
        return NULL;
      }
    }
    if (cgraph_add_edges(graph, edges) != CGRAPH_SUCCESS) {
      CGRAPH_WARNING("Lỗi thêm cạnh.");
      return NULL;
    }
  }

  return graph;
}

void cgraph_print(cgraph_t const g) {
  printf("n = %lld\n", (long long)cgraph_vcount(g));
  printf("có hướng? = %s\n", cgraph_is_directed(g)? "true": "false");
#define PRINT_ARR(v) \
  do { \
    printf("\t{%d", v[0]); \
    for (int i = 1; i < arr_size(v); ++i) { \
      printf(", %d", v[i]); \
    } \
    printf("}\n"); \
  } while (0)
  printf("from = ");
  PRINT_ARR(g->from);
  printf("to = ");
  PRINT_ARR(g->to);
  printf("oi = ");
  PRINT_ARR(g->oi);
  printf("ii = ");
  PRINT_ARR(g->ii);
  printf("os = ");
  PRINT_ARR(g->os);
  printf("is = ");
  PRINT_ARR(g->is);
#undef PRINT_ARR
}
