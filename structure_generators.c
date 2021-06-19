#include "cgraph_constructors.h"
#include "cgraph_error.h"
#include "cgraph_interface.h"
#include "cgraph_ivec.h"

#include <stdlib.h>
#include <stdio.h>

/**
 * \ingroup interface
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
    CGRAPH_ERROR("cannot create empty graph with negative number of vertices", CGRAPH_FAILURE);
    return NULL;
  }

  cgraph_t graph = malloc(sizeof(struct cgraph_s));

  graph->n = 0;
  graph->directed = directed;
  graph->from = cgraph_ivec_create();
  graph->to = cgraph_ivec_create();
  graph->oi = cgraph_ivec_create();
  graph->ii = cgraph_ivec_create();
  graph->os = cgraph_ivec_create();
  graph->is = cgraph_ivec_create();

  cgraph_ivec_push_back(&graph->os, 0);
  cgraph_ivec_push_back(&graph->is, 0);

  /* add the vertices */
  CGRAPH_CHECK(cgraph_add_vertices(graph, n));

  return graph;
}

cgraph_t cgraph_create(cgraph_ivec_t const edges,
          CGRAPH_INTEGER n,
          bool directed) {
  bool has_edges = cgraph_ivec_size(edges) > 0;
  CGRAPH_INTEGER max = has_edges ? cgraph_ivec_max(edges) + 1 : 0;

  if (cgraph_ivec_size(edges) % 2 != 0) {
    CGRAPH_ERROR("Invalid (odd) edges vector", CGRAPH_FAILURE);
    return NULL;
  }
  if (has_edges && !cgraph_ivec_isininterval(edges, 0, max - 1)) {
    CGRAPH_ERROR("Invalid (negative) vertex id", CGRAPH_FAILURE);
    return NULL;
  }

  cgraph_t graph = cgraph_create_empty(n, directed);

  if (has_edges) {
    CGRAPH_INTEGER vc = cgraph_vcount(graph);
    if (vc < max) {
      CGRAPH_CHECK(cgraph_add_vertices(graph, (CGRAPH_INTEGER) (max - vc)));
    }
    CGRAPH_CHECK(cgraph_add_edges(graph, edges));
  }

  return graph;
}

void cgraph_print(cgraph_t const g) {
  printf("n = %lld\n", (long long)cgraph_vcount(g));
  printf("directed = %s\n", cgraph_is_directed(g)? "true": "false");
#define PRINT_IVEC(v) \
  do { \
    printf("\t{%d", v[0]); \
    for (int i = 1; i < cgraph_ivec_size(v); ++i) { \
      printf(", %d", v[i]); \
    } \
    printf("}\n"); \
  } while (0)
  printf("from = ");
  PRINT_IVEC(g->from);
  printf("to = ");
  PRINT_IVEC(g->to);
  printf("oi = ");
  PRINT_IVEC(g->oi);
  printf("ii = ");
  PRINT_IVEC(g->ii);
  printf("os = ");
  PRINT_IVEC(g->os);
  printf("is = ");
  PRINT_IVEC(g->is);
#undef PRINT_IVEC
}
