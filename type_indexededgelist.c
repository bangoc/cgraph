#include "cgraph_error.h"
#include "cgraph_interface.h"
#include "cgraph_ivec.h"

#include <stdlib.h>

/**
 * \ingroup interface
 * \function cgraph_vcount
 * \brief Số lượng đỉnh trong một đồ thị.
 *
 * \param graph Biểu diễn đồ thị.
 * \return Số lượng đỉnh.
 *
 * Độ phức tạp: O(1)
 */
;
CGRAPH_INTEGER cgraph_vcount(const cgraph_t graph) {
  return graph->n;
}

/**
 * \ingroup interface
 * \function cgraph_ecount
 * \brief Số lượng cạnh của đồ thị.
 *
 * \param graph Biểu diễn đồ thị.
 * \return Số lượng cạnh.
 *
 * Độ phức tạp: O(1)
 */
CGRAPH_INTEGER cgraph_ecount(const cgraph_t graph) {
  return (CGRAPH_INTEGER) cgraph_ivec_size(graph->from);
}

/**
 * \ingroup interface
 * \function cgraph_is_directed
 * \brief Đồ thị có phải là đồ thị có hướng hay không?
 *
 * \param graph Biểu diễn đồ thị.
 * \return Giá trị lô-gic, <code>TRUE</code> nếu là đồ thị có hướng,
 * <code>FALSE</code> nếu ngược lại.
 *
 * Độ phức tạp: O(1)
 *
 */

bool cgraph_is_directed(const cgraph_t graph) {
    return graph->directed;
}

/*
 * Hàm sử dụng bên trong thư viện
 */

static int cgraph_i_create_start(
        cgraph_ivec_t res, cgraph_ivec_t el,
        cgraph_ivec_t iindex, CGRAPH_INTEGER nodes) {

# define EDGE(i) el[ iindex[(i)] ]

    CGRAPH_INTEGER no_of_nodes;
    CGRAPH_INTEGER no_of_edges;
    CGRAPH_INTEGER i, j, idx;

    no_of_nodes = nodes;
    no_of_edges = cgraph_ivec_size(el);

    /* kết quả */

    cgraph_ivec_setsize(res, nodes + 1);

    /* tạo chỉ mục */

    if (cgraph_ivec_size(el) == 0) {
        /* đồ thị rỗng */
        cgraph_ivec_null(res);
    } else {
        idx = -1;
        for (i = 0; i <= EDGE(0); i++) {
            idx++; res[idx] = 0;
        }
        for (i = 1; i < no_of_edges; i++) {
            CGRAPH_INTEGER n =
              (CGRAPH_INTEGER) (EDGE(i) - EDGE(res[idx]));
            for (j = 0; j < n; j++) {
                idx++; res[idx] = i;
            }
        }
        j = EDGE(res[idx]);
        for (i = 0; i < no_of_nodes - j; i++) {
            idx++; res[idx] = no_of_edges;
        }
    }

    /* dọn dẹp */

# undef EDGE
    return 0;
}

/**
 * \ingroup interface
 * \function cgraph_add_edges
 * \brief Thêm các cạnh vào một đồ thị.
 *
 * </para><para>
 * Các cạnh được cho trong một vec-tơ, hai phần tử đầu tiên xác định
 * cạnh đầu tiên theo thứ tự <code>from</code>, <code>to</code> đối
 * với đồ thị có hướng. Vec-tơ phải chứa một số lượng chẵn các số
 * nguyên trong khoảng giữa 0 và số lượng đỉnh trong đồ thị trừ một
 * (bao gồm). Nếu bạn muốn thêm các đỉnh mới, hãy gọi hàm
 * cgraph_add_vertices trước.
 * \param graph Đồ thị mà các cạnh sẽ được thêm vào.
 * \param edges Các cạnh.
 * \return Mã lỗi:
 *    \c CGRAPH_FAILURE: Không thể thêm cạnh do độ dài vec-tơ
 *       edges không hợp lệ, hoặc có đỉnh ngoài phạm vi, v.v..
 *    \c CGRAPH_SUCCESS: Nếu thành công
 *
 *
 * </para><para>
 * Độ phức tạp: O(|V|+|E|) Trong đó
 * |V| là số lượng đỉnh và
 * |E| là số lượng cạnh trong đồ thị được mở rộng.
 *
 * \example examples/simple/igraph_add_edges.c
 * TODO: Bổ xung ví dụ cho cgraph
 */
int cgraph_add_edges(cgraph_t graph, cgraph_ivec_t const edges) {
    long int no_of_edges = cgraph_ecount(graph);
    long int edges_to_add = cgraph_ivec_size(edges) / 2;
    long int i = 0;
    cgraph_error_handler_t *oldhandler;
    bool ret1, ret2;
    bool directed = cgraph_is_directed(graph);

    if (cgraph_ivec_size(edges) % 2 != 0) {
        CGRAPH_ERROR(
          "Lỗi độ dài vec-tơ cạnh (lẻ)", CGRAPH_FAILURE);
    }
    if (!cgraph_ivec_isininterval(edges,
              0, cgraph_vcount(graph) - 1)) {
        CGRAPH_ERROR(
          "Không thể thêm cạnh", CGRAPH_FAILURE);
    }

    cgraph_ivec_t newoi = cgraph_ivec_create(),
                  newii = cgraph_ivec_create();

    /* from & to */
    CGRAPH_CHECK(cgraph_ivec_grow(&graph->from,
                      no_of_edges + edges_to_add));
    CGRAPH_CHECK(cgraph_ivec_grow(&graph->to,
                      no_of_edges + edges_to_add));

    /*
     * Nếu là đồ thị có hướng thì from - to được xác định theo đúng
     * thứ tự trong vec-tơ edges;
     * Nếu ngược lại (trong trường hợp đồ thị vô hướng)
     *   thì from là giá trị cực đại trong hai đỉnh,
     *   còn to là giá trị còn lại
     */
    while (i < edges_to_add * 2) {
        if (directed || edges[i] > edges[i + 1]) {
            cgraph_ivec_push_back(&graph->from, edges[i++]);
            cgraph_ivec_push_back(&graph->to,   edges[i++]);
        } else {
            cgraph_ivec_push_back(&graph->to,   edges[i++]);
            cgraph_ivec_push_back(&graph->from, edges[i++]);
        }
    }

    /* Tạm thời vô hiệu xử lý lỗi */
    oldhandler =
        cgraph_set_error_handler(cgraph_error_handler_ignore);

    /* oi & ii */
    ret1 = cgraph_ivec_init(&newoi, no_of_edges + edges_to_add);
    ret2 = cgraph_ivec_init(&newii, no_of_edges + edges_to_add);
    if (ret1 != 0 || ret2 != 0) {
      /* Thu gọn vec-tơ */
      cgraph_ivec_setsize(graph->from, no_of_edges);
      cgraph_ivec_setsize(graph->to, no_of_edges);

      /* Khôi phục xử lý lỗi */
      cgraph_set_error_handler(oldhandler);

      cgraph_ivec_free(&newoi);
      cgraph_ivec_free(&newii);
      CGRAPH_ERROR("Không thể thêm cạnh", CGRAPH_FAILURE);
    }
    ret1 = cgraph_ivec_order(graph->from, graph->to, newoi);
    ret2 = cgraph_ivec_order(graph->to, graph->from, newii);
    if (ret1 != 0 || ret2 != 0) {
        cgraph_ivec_setsize(graph->from, no_of_edges);
        cgraph_ivec_setsize(graph->to, no_of_edges);
        cgraph_ivec_free(&newoi);
        cgraph_ivec_free(&newii);
        cgraph_set_error_handler(oldhandler);
        CGRAPH_ERROR("Không thể thêm cạnh", CGRAPH_FAILURE);
    }

    /* os & is, độ dài của nó không thay đổi, thao tác an toàn */
    cgraph_i_create_start(graph->os, graph->from, newoi, graph->n);
    cgraph_i_create_start(graph->is, graph->to, newii, graph->n);

    /* Tất cả đều đã ổn  */
    cgraph_ivec_free(&graph->oi);
    cgraph_ivec_free(&graph->ii);
    graph->oi = newoi;
    graph->ii = newii;
    cgraph_set_error_handler(oldhandler);

    return CGRAPH_SUCCESS;
}

/**
 * \ingroup interface
 * \function cgraph_add_vertices
 * \brief Thêm các đỉnh vào đồ thị.
 *
 * </para><para>
 *
 * \param graph Đối tượng đồ thị cần được mở rộng.
 * \param nv Số nguyên không âm - là số lượng đỉnh cần được thêm vào
 * \return Mã lỗi:
 *     \c CGRAPH_FAILURE: Số lượng đỉnh mới không hợp lệ.
 *
 * Độ phức tạp: O(|V|) trong đó
 * |V| là
 * số lượng đỉnh trong đồ thì sau khi mở rộng.
 *
 * \example examples/simple/igraph_add_vertices.c
 * TODO: Bổ xung ví dụ cho cgraph
 */
int cgraph_add_vertices(cgraph_t graph, CGRAPH_INTEGER nv) {
  long int ec = cgraph_ecount(graph);
  long int i;

  if (nv < 0) {
    CGRAPH_ERROR("Không thể thêm số lượng đỉnh âm", CGRAPH_FAILURE);
  }

  CGRAPH_CHECK(cgraph_ivec_grow(&graph->os, graph->n + nv + 1));
  CGRAPH_CHECK(cgraph_ivec_grow(&graph->is, graph->n + nv + 1));
  cgraph_ivec_setsize(graph->os, graph->n + nv + 1);
  cgraph_ivec_setsize(graph->is, graph->n + nv + 1);

  for (i = graph->n + 1; i < graph->n + nv + 1; i++) {
    (graph->os)[i] = ec;
    (graph->is)[i] = ec;
  }

  graph->n += nv;

  return 0;
}

/**
 * \ingroup interface
 * \function cgraph_destroy
 * \brief Giải phóng bộ nhớ đã được cấp phát cho một đồ thị.
 *
 * </para><para>
 * Hàm này chỉ được gọi đúng một lần cho một đồ thị.
 *
 * </para><para>
 * Bộ nhớ được cấp phát cho biểu diễn đồ thị phải được hủy trước khi
 * hủy bộ nhớ cho chính cấu trúc đồ thị.
 *
 * \param graph Con trỏ tới đồ thị cần giải phóng.
 *
 * Độ phức tạp: phụ thuộc vào hệ điều hành.
 */
void cgraph_destroy(cgraph_t *graph) {
  cgraph_t g = *graph;
  cgraph_ivec_free(&g->from);
  cgraph_ivec_free(&g->to);
  cgraph_ivec_free(&g->oi);
  cgraph_ivec_free(&g->ii);
  cgraph_ivec_free(&g->os);
  cgraph_ivec_free(&g->is);
  free(g);
  *graph = NULL;
}

int cgraph_neighbors(const cgraph_t graph,
                     cgraph_ivec_t *neis,
                     CGRAPH_INTEGER vid,
                     cgraph_neimode_t mode) {
  const CGRAPH_INTEGER node = vid;

  if (node < 0 || node > cgraph_vcount(graph) - 1) {
    CGRAPH_ERROR("Không thể truy cập các láng giềng", CGRAPH_FAILURE);
  }
  if (mode != CGRAPH_OUT && mode != CGRAPH_IN &&
      mode != CGRAPH_ALL) {
    CGRAPH_ERROR("Không thể truy cập các láng giềng", CGRAPH_FAILURE);
  }

  if (! graph->directed) {
      mode = CGRAPH_ALL;
  }

  cgraph_ivec_setsize(*neis, 0);

  if (!cgraph_is_directed(graph) || mode != CGRAPH_ALL) {
    if (mode & CGRAPH_OUT) {
      CGRAPH_INTEGER j = (graph->os)[node + 1];
      for (CGRAPH_INTEGER i = (graph->os)[node]; i < j; i++) {
        cgraph_ivec_push_back(neis, (graph->to)[ (graph->oi)[i] ]);
      }
    }
    if (mode & CGRAPH_IN) {
      CGRAPH_INTEGER j = (graph->is)[node + 1];
      for (CGRAPH_INTEGER i = (graph->is)[node]; i < j; i++) {
        cgraph_ivec_push_back(neis, (graph->from)[ (graph->ii)[i] ]);
      }
    }
  } else {
    /* Gom các láng giềng theo chiều vào- và ra- trong một
     * đồ thị có hướng.
     */
    CGRAPH_INTEGER j1 = (graph->os)[node + 1];
    CGRAPH_INTEGER j2 = (graph->is)[node + 1];
    CGRAPH_INTEGER i1 = (graph->os)[node];
    CGRAPH_INTEGER i2 = (graph->is)[node];
    while (i1 < j1 && i2 < j2) {
      CGRAPH_INTEGER n1 = (graph->to)[ (graph->oi)[i1] ];
      CGRAPH_INTEGER n2 = (graph->from)[ (graph->ii)[i2] ];
      if (n1 < n2) {
        cgraph_ivec_push_back(neis, n1);
        i1++;
      } else if (n1 > n2) {
        cgraph_ivec_push_back(neis, n2);
        i2++;
      } else {
        cgraph_ivec_push_back(neis, n1);
        cgraph_ivec_push_back(neis, n2);
        i1++;
        i2++;
      }
    }
    while (i1 < j1) {
      CGRAPH_INTEGER n1 = (graph->to)[ (graph->oi)[i1] ];
      cgraph_ivec_push_back(neis, n1);
      i1++;
    }
    while (i2 < j2) {
      CGRAPH_INTEGER n2 = (graph->from)[ (graph->ii)[i2] ];
      cgraph_ivec_push_back(neis, n2);
      i2++;
    }
  }

  return 0;
}

int cgraph_incident(const cgraph_t graph,
                    cgraph_ivec_t *eids,
                    CGRAPH_INTEGER vid,
                    cgraph_neimode_t mode) {
  const CGRAPH_INTEGER node = vid;
  if (node < 0 || node > cgraph_vcount(graph) - 1) {
    CGRAPH_ERROR("Không thể truy cập các láng giềng", CGRAPH_FAILURE);
  }
  if (mode != CGRAPH_OUT && mode != CGRAPH_IN &&
      mode != CGRAPH_ALL) {
    CGRAPH_ERROR("Không thể truy cập các láng giềng", CGRAPH_FAILURE);
  }

  if (! graph->directed) {
    mode = CGRAPH_ALL;
  }

  cgraph_ivec_setsize(*eids, 0);

  if (mode & CGRAPH_OUT) {
    CGRAPH_INTEGER j = (graph->os)[node + 1];
    for (CGRAPH_INTEGER i = (graph->os)[node]; i < j; i++) {
      cgraph_ivec_push_back(eids, (graph->oi)[i]);
    }
  }
  if (mode & CGRAPH_IN) {
    CGRAPH_INTEGER j = (graph->is)[node + 1];
    for (CGRAPH_INTEGER i = (graph->is)[node]; i < j; i++) {
      cgraph_ivec_push_back(eids, (graph->ii)[i]);
    }
  }
  return 0;
}

int cgraph_degree_all(const cgraph_t graph,
                      cgraph_ivec_t *res,
                      cgraph_neimode_t mode,
                      bool loops) {
  if (mode != CGRAPH_OUT && mode != CGRAPH_IN &&
      mode != CGRAPH_ALL) {
    CGRAPH_ERROR("Không thể lấy bậc", CGRAPH_FAILURE);
  }
  if (!cgraph_is_directed(graph)) {
    mode = CGRAPH_ALL;
  }
  CGRAPH_INTEGER no_of_nodes = cgraph_vcount(graph);
  CGRAPH_INTEGER no_of_edges = cgraph_ecount(graph);
  cgraph_ivec_init(res, no_of_nodes);
  cgraph_ivec_t v = *res;
  cgraph_ivec_fill(v, 0);

  for (CGRAPH_INTEGER ed = 0; ed < no_of_edges; ++ed) {
    if (loops || graph->from[ed] != graph->to[ed]) {
      if (mode & CGRAPH_IN) {
        v[ graph->to[ed] ] += 1;
      }
      if (mode & CGRAPH_OUT) {
        v[ graph->from[ed] ] += 1;
      }
    }
  }
  return 0;
}

int cgraph_degree_one(const cgraph_t graph,
                      CGRAPH_INTEGER *res,
                      const CGRAPH_INTEGER node,
                      cgraph_neimode_t mode,
                      bool loops) {
  if (node < 0 || node > cgraph_vcount(graph) - 1) {
    CGRAPH_ERROR("Không thể lấy bậc", CGRAPH_FAILURE);
  }
  if (mode != CGRAPH_OUT && mode != CGRAPH_IN &&
      mode != CGRAPH_ALL) {
    CGRAPH_ERROR("Không thể lấy bậc", CGRAPH_FAILURE);
  }
  if (!cgraph_is_directed(graph)) {
    mode = CGRAPH_ALL;
  }
  CGRAPH_INTEGER d = 0;
  if (mode & CGRAPH_IN) {
    d += (graph->is[node + 1] - graph->is[node]);
  }
  if (mode & CGRAPH_OUT) {
    d += (graph->os[node + 1] - graph->os[node]);
  }
  if (!loops) {
    if (mode & CGRAPH_IN) {
      CGRAPH_INTEGER j = graph->is[node];
      CGRAPH_INTEGER j1 = graph->is[node + 1];
      for (CGRAPH_INTEGER i = j; i < j1; ++i) {
        CGRAPH_INTEGER idx = graph->ii[i];
        if (graph->from[ idx ] == graph->to[ idx ]) {
          --d;
        }
      }
    }
    if (mode & CGRAPH_OUT) {
      CGRAPH_INTEGER j = graph->os[node];
      CGRAPH_INTEGER j1 = graph->os[node + 1];
      for (CGRAPH_INTEGER i = j; i < j1; ++i) {
        CGRAPH_INTEGER idx = graph->oi[i];
        if (graph->from[ idx ] == graph->to[ idx ]) {
          --d;
        }
      }
    }
  }
  *res = d;
  return 0;
}

/**
 * \function cgraph_edge
 * \brief Trả về các đỉnh nguồn và đích của một cạnh.
 *
 * \param graph Đối tượng đồ thị.
 * \param eid Chỉ số của cạnh.
 * \param from Con trỏ tới một biến CGRAPH_INTEGER. Đỉnh nguồn của
 * cạnh sẽ được lưu ở đây.
 * \param to Con trỏ tới một biến CGRAPH_INTEGER. Đỉnh đích của cạnh
 * sẽ được lưu ở đây.
 * \return Mã lỗi. Triển khai hiện tại luôn trả về mã thành công.
 * Đối với đồ thị vô hướng, trả về from <= to.
 * \sa \ref cgraph_get_eid() cho thao tác ngược lại;
 *     \ref CGRAPH_TO(), \ref CGRAPH_FROM() và \ref CGRAPH_OTHER()
 * cho phiên bản nhanh hơn nhưng không kiểm tra lỗi.
 *
 * Độ phức tạp thời gian: O(1).
 */

int cgraph_edge(const cgraph_t graph, CGRAPH_INTEGER eid,
               CGRAPH_INTEGER *from, CGRAPH_INTEGER *to) {
    if (cgraph_is_directed(graph)) {
        *from = (CGRAPH_INTEGER) (graph->from)[eid];
        *to   = (CGRAPH_INTEGER) (graph->to  )[eid];
    } else {
        *from = (CGRAPH_INTEGER) (graph->to  )[eid];
        *to   = (CGRAPH_INTEGER) (graph->from)[eid];
    }

    return 0;
}

/**
 * \function cgraph_get_eid
 * \brief Lấy chỉ số cạnh từ hai đỉnh của nó
 *
 * Đối với đồ thị vô hướng có thể hoán đổi vị trí \c pfrom và \c pto.
 *
 * \param graph Đối tượng đồ thị
 * \param eid Con trỏ tới một số nguyên, chỉ số cạnh sẽ được lưu ở
 * đây.
 * \param pfrom Đỉnh nguồn của cạnh.
 * \param pto Đỉnh đích của cạnh.
 * \param directed Tùy chỉnh tính đến hướng của cạnh hoặc không trong
 * đồ thị có hướng. Bị bỏ qua đối với đồ thị vô hướng.
 * \return Mã lỗi.
 * \sa \ref cgraph_edge() thao tác ngược lại.
 *
 * Độ phức tạp: O(log(d)), trong đó d là giá trị nhỏ hơn giữa bậc
 * ra của \c pfrom và bậc vào của \c pto nếu \p directed bằng true.
 * Nếu \p directed bằng false, thì độ phức tạp là
 * O(log(d) + log(d2)), trong đó d vẫn như trường hợp trước và d2 là
 * giá trị nhỏ hơn giữa bậc ra của \c pto và bậc vào của \c pfrom.
 *
 */

#define BINSEARCH(start,end,value,iindex,edgelist,N,pos) \
  do {                                                   \
    while ((start) < (end)) {                            \
      CGRAPH_INTEGER mid = (start)+((end)-(start))/2;    \
      CGRAPH_INTEGER e = (iindex)[mid];                  \
      if ((edgelist)[e] < (value)) {                     \
        (start)=mid+1;                                   \
      } else {                                           \
        (end)=mid;                                       \
      }                                                  \
    }                                                    \
    if ((start)<(N)) {                                   \
      CGRAPH_INTEGER e=(iindex)[(start)];                \
      if ((edgelist)[e] == (value)) {                    \
        *(pos)=(CGRAPH_INTEGER) e;                       \
      }                                                  \
    }                                                    \
  } while(0)

#define FIND_DIRECTED_EDGE(graph,xfrom,xto,eid)          \
    do {                                                 \
        CGRAPH_INTEGER start = (graph->os)[xfrom];       \
        CGRAPH_INTEGER end = (graph->os)[xfrom+1];       \
        CGRAPH_INTEGER N = end;                          \
        CGRAPH_INTEGER start2 = (graph->is)[xto];        \
        CGRAPH_INTEGER end2 = (graph->is)[xto+1];        \
        CGRAPH_INTEGER N2 = end2;                        \
        if (end - start < end2 - start2) {                                \
            BINSEARCH(start,end,xto,graph->oi,graph->to,N,eid);           \
        } else {                                                          \
            BINSEARCH(start2,end2,xfrom,graph->ii,graph->from,N2,eid);    \
        }                                                                 \
    } while (0)

#define FIND_UNDIRECTED_EDGE(graph,from,to,eid)         \
    do {                                                \
        CGRAPH_INTEGER xfrom1= from > to ? from : to;   \
        CGRAPH_INTEGER xto1= from > to ? to : from;     \
        FIND_DIRECTED_EDGE(graph,xfrom1,xto1,eid);      \
    } while (0)

int cgraph_get_eid(const cgraph_t graph, CGRAPH_INTEGER *eid,
                   CGRAPH_INTEGER pfrom, CGRAPH_INTEGER pto,
                   bool directed) {

    CGRAPH_INTEGER from = pfrom, to = pto;
    CGRAPH_INTEGER nov = cgraph_vcount(graph);

    if (from < 0 || to < 0 || from > nov - 1 || to > nov - 1) {
      CGRAPH_WARNING("Không thể lấy mã cạnh, lỗi dữ liệu vào");
    }

    *eid = -1;
    if (cgraph_is_directed(graph)) {

      /* Đồ thị có hướng */
      FIND_DIRECTED_EDGE(graph, from, to, eid);
      if (!directed && *eid < 0) {
        FIND_DIRECTED_EDGE(graph, to, from, eid);
      }
    } else {
      /* Đồ thị vô hướng chỉ có một chế độ */
      FIND_UNDIRECTED_EDGE(graph, from, to, eid);
    }

    if (*eid < 0) {
      CGRAPH_WARNING("Không thể lấy mã cạnh, không tìm thấy cạnh");
    }

    return 0;
}
