#include "base/cgraph_arr.h"
#include "base/cgraph_error.h"
#include "cgraph_interface.h"

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
  return (CGRAPH_INTEGER) arr_size(graph->from);
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
        atype(CGRAPH_INTEGER) *res,
        atype(CGRAPH_INTEGER) *el,
        atype(CGRAPH_INTEGER) *iindex,
        CGRAPH_INTEGER nodes) {

# define EDGE(i) el[ iindex[(i)] ]

  CGRAPH_INTEGER no_of_nodes;
  CGRAPH_INTEGER no_of_edges;
  CGRAPH_INTEGER i, j, idx;

  no_of_nodes = nodes;
  no_of_edges = arr_size(el);

  /* kết quả */

  arr_resize(res, nodes + 1);

  /* tạo chỉ mục */

  if (arr_size(el) == 0) {
    /* đồ thị rỗng */
    arr_ifill(res, 0);
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
 * \return Không trả về giá trị
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
void cgraph_add_edges(cgraph_t graph, atype(CGRAPH_INTEGER) *edges) {
  cgraph_err_reset();
  long int no_of_edges = cgraph_ecount(graph);
  long int edges_to_add = arr_size(edges) / 2;
  long int i = 0;
  cgraph_error_handler_t *oldhandler;
  bool ret1, ret2;
  bool directed = cgraph_is_directed(graph);

  if (arr_size(edges) % 2 != 0) {
    CGRAPH_ERROR(
      "Lỗi độ dài kích thước vec-tơ cạnh (lẻ)", CGRAPH_FAILURE);
    return;
  }
  if (!arr_irange(edges, 0, cgraph_vcount(graph) - 1)) {
    CGRAPH_ERROR("Chỉ số đỉnh nằm ngoài khoảng", CGRAPH_FAILURE);
    return;
  }

  arr_make(newoi, 0, CGRAPH_INTEGER);
  arr_make(newii, 0, CGRAPH_INTEGER);

  /* from & to */
  arr_reserve(graph->from, no_of_edges + edges_to_add);
  arr_reserve(graph->to, no_of_edges + edges_to_add);

  /*
   * Nếu là đồ thị có hướng thì from - to được xác định theo đúng
   * thứ tự trong vec-tơ edges;
   * Nếu ngược lại (trong trường hợp đồ thị vô hướng)
   *   thì from là giá trị cực đại trong hai đỉnh,
   *   còn to là giá trị còn lại
   */
  while (i < edges_to_add * 2) {
    if (directed || edges[i] > edges[i + 1]) {
      arr_append(graph->from, edges[i++]);
      arr_append(graph->to,   edges[i++]);
    } else {
      arr_append(graph->to,   edges[i++]);
      arr_append(graph->from, edges[i++]);
    }
  }

  /* Tạm thời vô hiệu xử lý lỗi */
  oldhandler =
    cgraph_set_error_handler(cgraph_error_handler_ignore);

  /* oi & ii */
  arr_resize(newoi, no_of_edges + edges_to_add);
  arr_resize(newii, no_of_edges + edges_to_add);

  // TODO: Xử lý lỗi
  // if (ret1 != 0 || ret2 != 0) {
  //   // Thu gọn vec-tơ
  //   cgraph_ivec_setsize(graph->from, no_of_edges);
  //   cgraph_ivec_setsize(graph->to, no_of_edges);

  //   // Khôi phục xử lý lỗi
  //   cgraph_set_error_handler(oldhandler);

  //   cgraph_ivec_free(&newoi);
  //   cgraph_ivec_free(&newii);
  //   CGRAPH_ERROR("Không thể thêm cạnh", CGRAPH_FAILURE);
  // }

  arr_iorder(graph->from, graph->to, newoi);
  arr_iorder(graph->to, graph->from, newii);

  // TODO: Khôi phục xử lý lỗi
  // if (ret1 != 0 || ret2 != 0) {
  //   cgraph_ivec_setsize(graph->from, no_of_edges);
  //   cgraph_ivec_setsize(graph->to, no_of_edges);
  //   cgraph_ivec_free(&newoi);
  //   cgraph_ivec_free(&newii);
  //   cgraph_set_error_handler(oldhandler);
  //   CGRAPH_ERROR("Không thể thêm cạnh", CGRAPH_FAILURE);
  // }

  /* os & is, độ dài của nó không thay đổi, thao tác an toàn */
  cgraph_i_create_start(graph->os, graph->from, newoi, graph->n);
  cgraph_i_create_start(graph->is, graph->to, newii, graph->n);

  /* Tất cả đều đã ổn  */
  arr_free(graph->oi);
  arr_free(graph->ii);
  graph->oi = newoi;
  graph->ii = newii;
  cgraph_set_error_handler(oldhandler);

  return;
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
 * \return Không trả về giá trị
 *
 * Độ phức tạp: O(|V|) trong đó
 * |V| là
 * số lượng đỉnh trong đồ thì sau khi mở rộng.
 *
 * \example examples/simple/igraph_add_vertices.c
 * TODO: Bổ xung ví dụ cho cgraph
 */
void cgraph_add_vertices(cgraph_t graph, CGRAPH_INTEGER nv) {
  cgraph_err_reset();
  long int ec = cgraph_ecount(graph);
  long int i;

  if (nv < 0) {
    CGRAPH_ERROR("Không thể thêm số lượng đỉnh âm", CGRAPH_FAILURE);
    return;
  }

  arr_resize(graph->os, graph->n + nv + 1);
  arr_resize(graph->is, graph->n + nv + 1);

  for (i = graph->n + 1; i < graph->n + nv + 1; i++) {
    (graph->os)[i] = ec;
    (graph->is)[i] = ec;
  }

  graph->n += nv;
}

atype(CGRAPH_INTEGER) *cgraph_neighbors(const cgraph_t graph,
     CGRAPH_INTEGER vid, cgraph_neimode_t mode) {
  cgraph_err_reset();
  const CGRAPH_INTEGER node = vid;

  if (node < 0 || node > cgraph_vcount(graph) - 1) {
    CGRAPH_ERROR("Chỉ số đỉnh nằm ngoài khoảng", CGRAPH_FAILURE);
    return NULL;
  }
  if (mode != CGRAPH_OUT && mode != CGRAPH_IN && mode != CGRAPH_ALL) {
    CGRAPH_ERROR("Tham số mode không hợp lệ", CGRAPH_FAILURE);
  return NULL;
  }

  if (!graph->directed) {
      mode = CGRAPH_ALL;
  }

  arr_make(neis, 0, CGRAPH_INTEGER);
  if (!cgraph_is_directed(graph) || mode != CGRAPH_ALL) {
    if (mode & CGRAPH_OUT) {
      CGRAPH_INTEGER j = (graph->os)[node + 1];
      for (CGRAPH_INTEGER i = (graph->os)[node]; i < j; i++) {
        arr_append(neis, (graph->to)[ (graph->oi)[i] ]);
      }
    }
    if (mode & CGRAPH_IN) {
      CGRAPH_INTEGER j = (graph->is)[node + 1];
      for (CGRAPH_INTEGER i = (graph->is)[node]; i < j; i++) {
        arr_append(neis, (graph->from)[ (graph->ii)[i] ]);
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
        arr_append(neis, n1);
        i1++;
      } else if (n1 > n2) {
        arr_append(neis, n2);
        i2++;
      } else {
        arr_append(neis, n1);
        arr_append(neis, n2);
        i1++;
        i2++;
      }
    }
    while (i1 < j1) {
      CGRAPH_INTEGER n1 = (graph->to)[ (graph->oi)[i1] ];
      arr_append(neis, n1);
      i1++;
    }
    while (i2 < j2) {
      CGRAPH_INTEGER n2 = (graph->from)[ (graph->ii)[i2] ];
      arr_append(neis, n2);
      i2++;
    }
  }

  return neis;
}

atype(CGRAPH_INTEGER) *cgraph_incident(const cgraph_t graph,
    CGRAPH_INTEGER vid, cgraph_neimode_t mode) {
  cgraph_err_reset();
  const CGRAPH_INTEGER node = vid;
  if (node < 0 || node > cgraph_vcount(graph) - 1) {
    CGRAPH_ERROR("Tham số đỉnh ngoài khoảng", CGRAPH_FAILURE);
    return NULL;
  }
  if (mode != CGRAPH_OUT && mode != CGRAPH_IN && mode != CGRAPH_ALL) {
    CGRAPH_ERROR("Sai tham số mode", CGRAPH_FAILURE);
    return NULL;
  }

  if (!graph->directed) {
    mode = CGRAPH_ALL;
  }

  arr_make(eids, 0, CGRAPH_INTEGER);
  if (mode & CGRAPH_OUT) {
    CGRAPH_INTEGER j = (graph->os)[node + 1];
    for (CGRAPH_INTEGER i = (graph->os)[node]; i < j; i++) {
      arr_append(eids, (graph->oi)[i]);
    }
  }
  if (mode & CGRAPH_IN) {
    CGRAPH_INTEGER j = (graph->is)[node + 1];
    for (CGRAPH_INTEGER i = (graph->is)[node]; i < j; i++) {
      arr_append(eids, (graph->ii)[i]);
    }
  }
  return eids;
}

atype(CGRAPH_INTEGER) *cgraph_degree_all(const cgraph_t graph,
                      cgraph_neimode_t mode, bool loops) {
  cgraph_err_reset();
  if (mode != CGRAPH_OUT && mode != CGRAPH_IN && mode != CGRAPH_ALL) {
    CGRAPH_ERROR("Lỗi tham số mode", CGRAPH_FAILURE);
    return NULL;
  }
  if (!cgraph_is_directed(graph)) {
    mode = CGRAPH_ALL;
  }
  CGRAPH_INTEGER no_of_nodes = cgraph_vcount(graph);
  CGRAPH_INTEGER no_of_edges = cgraph_ecount(graph);
  arr_make(v, no_of_nodes, CGRAPH_INTEGER);
  arr_fill(v, 0);

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
  return v;
}

CGRAPH_INTEGER cgraph_degree_one(const cgraph_t graph,
                      const CGRAPH_INTEGER node,
                      cgraph_neimode_t mode,
                      bool loops) {
  cgraph_err_reset();
  if (node < 0 || node > cgraph_vcount(graph) - 1) {
    CGRAPH_ERROR("Chỉ số nút ngoài khoảng", CGRAPH_FAILURE);
    return -1;
  }
  if (mode != CGRAPH_OUT && mode != CGRAPH_IN &&
      mode != CGRAPH_ALL) {
    CGRAPH_ERROR("Không thể lấy bậc", CGRAPH_FAILURE);
    return -1;
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
  return d;
}

/**
 * \function cgraph_edge
 * \brief Trả về các đỉnh nguồn và đích của một cạnh.
 *
 * \param graph Đối tượng đồ thị.
 * \param eid Chỉ số của cạnh.
 * \return 1 đối tượng struct edge chứa cặp đỉnh from và to.
 * Đối với đồ thị vô hướng, trả về from <= to.
 * \sa \ref cgraph_get_eid() cho thao tác ngược lại;
 *     \ref CGRAPH_TO(), \ref CGRAPH_FROM() và \ref CGRAPH_OTHER()
 * cho phiên bản nhanh hơn nhưng không kiểm tra lỗi.
 *
 * Độ phức tạp thời gian: O(1).
 */
struct edge cgraph_edge(const cgraph_t graph, CGRAPH_INTEGER eid) {
  cgraph_err_reset();
  struct edge e;
  if (cgraph_is_directed(graph)) {
    e.from = (CGRAPH_INTEGER) (graph->from)[eid];
    e.to   = (CGRAPH_INTEGER) (graph->to  )[eid];
  } else {
    e.from = (CGRAPH_INTEGER) (graph->to  )[eid];
    e.to   = (CGRAPH_INTEGER) (graph->from)[eid];
  }

  return e;
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

CGRAPH_INTEGER cgraph_get_eid(const cgraph_t graph,
                   CGRAPH_INTEGER pfrom, CGRAPH_INTEGER pto, bool directed) {
  cgraph_err_reset();
  CGRAPH_INTEGER from = pfrom, to = pto;
  CGRAPH_INTEGER nov = cgraph_vcount(graph);

  if (from < 0 || to < 0 || from > nov - 1 || to > nov - 1) {
    CGRAPH_ERROR("Không thể lấy mã cạnh, chỉ số đỉnh ngoài khoảng", CGRAPH_FAILURE);
    return -1;
  }

  CGRAPH_INTEGER eid = -1;
  if (cgraph_is_directed(graph)) {

    /* Đồ thị có hướng */
    FIND_DIRECTED_EDGE(graph, from, to, &eid);
    if (!directed && eid < 0) {
      FIND_DIRECTED_EDGE(graph, to, from, &eid);
    }
  } else {
    /* Đồ thị vô hướng chỉ có một chế độ */
    FIND_UNDIRECTED_EDGE(graph, from, to, &eid);
  }

  if (eid < 0) {
    CGRAPH_WARNING("Không thể lấy mã cạnh, không tìm thấy cạnh");
  }

  return eid;
}


/**
 * \ingroup interface
 * \function cgraph_delete_edges
 * \brief Xóa các cạnh khỏi đồ thị.
 *
 * </para><para>
 * Các cạnh cần xóa được để trong vec-tơ.
 *
 * </para><para>
 * Hàm này không xóa đỉnh, kể cả tất cả các cạnh gắn với nó đã bị xóa
 *
 * </para><para>
 * Hàm này giải phóng bộ nhớ được cấp phát cho đồ thị.
 * \param graph Đồ thị được xử lý.
 * \param edges Các cạnh cần xóa.
 * \return Không trả về giá trị
 *
 * Độ phức tạp thời gian: O(|V|+|E|) trong đó
 * |V| và |E| tương ứng là số lượng đỉnh và số lượng cạnh trong đồ
 * thị ban đầu.
 *
 * \example examples/simple/igraph_delete_edges.c
 * TODO: Bổ xung ví dụ
 */
void cgraph_delete_edges(cgraph_t graph, atype(CGRAPH_INTEGER) *edges) {
  cgraph_err_reset();
  CGRAPH_INTEGER no_of_edges = cgraph_ecount(graph);
  CGRAPH_INTEGER no_of_nodes = cgraph_vcount(graph);
  CGRAPH_INTEGER edges_to_remove = 0;
  CGRAPH_INTEGER remaining_edges;

  arr_make(newfrom, 0, CGRAPH_INTEGER);
  arr_make(newto, 0, CGRAPH_INTEGER);

  CGRAPH_INTEGER *mark;
  CGRAPH_INTEGER i, j;

  mark = calloc(no_of_edges, sizeof(int));
  if (mark == 0) {
    CGRAPH_ERROR("Không thể cấp phát bộ nhớ", CGRAPH_FAILURE);
    return;
  }
  for (i = 0; i < arr_size(edges); ++i) {
    CGRAPH_INTEGER eid = edges[i];
    if (mark[eid] == 0) {
      edges_to_remove++;
      mark[eid]++;
    }
  }
  remaining_edges = no_of_edges - edges_to_remove;

  arr_resize(newfrom, remaining_edges);
  arr_resize(newto, remaining_edges);

  /* Lưu các cạnh không bị xóa vào newfrom và newto */
  for (i = 0, j = 0; j < remaining_edges; i++) {
    if (mark[i] == 0) {
      newfrom[j] = graph->from[i];
      newto[j] = graph->to[i];
      j++;
    }
  }

  /* Tạo các chỉ mục, thao tác này có thể cấp phát thêm bộ nhớ */
  arr_make(newoi, remaining_edges, CGRAPH_INTEGER);
  arr_make(newii, remaining_edges, CGRAPH_INTEGER);

  arr_iorder(newfrom, newto, newoi);
  arr_iorder(newto, newfrom, newii);

  /* Ok, chúng ta giải phóng bộ nhớ cho các cấu trúc cũ  */
  arr_free(graph->from);
  arr_free(graph->to);
  arr_free(graph->oi);
  arr_free(graph->ii);
  graph->from = newfrom;
  graph->to = newto;
  graph->oi = newoi;
  graph->ii = newii;

  free(mark);

  /* Tạo vec-tơ bắt đầu, không cần thêm bộ nhớ cho thao tác này */
  cgraph_i_create_start(graph->os, graph->from, graph->oi, no_of_nodes);
  cgraph_i_create_start(graph->is, graph->to, graph->ii, no_of_nodes);
}

/**
 * \ingroup interface
 * \function cgraph_disconnect_vertices
 * \brief Xóa các liên kết gắn với đỉnh trong danh sách.
 *        Hàm này không thay đổi chỉ số đỉnh. Nếu nhất định
 *        phải thay đổi chỉ số đỉnh thì bạn có thể tạo lại đồ thị.
 *        Hàm này có thay đổi chỉ số cạnh, tuy nhiên trật tự tương đối
 *        giữa các cạnh là không thay đổi.
 *
 * </para><para>
 * Hàm này có thay đổi bộ nhớ động.
 *
 * \param graph Đồ thị cần xử lý.
 * \param vertices Vec-tơ chứa id của các đỉnh cần ngắt kết nối.
 * \return Không trả về giá trị.
 *
 * Độ phức tạp thời gian: O(|V|+|E|),
 * |V| và |E| là số lượng đỉnh và số lượng cạnh trong đồ thị ban đầu.
 *
 */
void cgraph_disconnect_vertices(cgraph_t graph,
      atype(CGRAPH_INTEGER) *vertices, cgraph_neimode_t mode) {
  cgraph_err_reset();
  CGRAPH_INTEGER no_of_vertices = cgraph_vcount(graph);
  CGRAPH_INTEGER no_of_edges = cgraph_ecount(graph);
  char *vmark = calloc(no_of_vertices, sizeof(char)),
       *emark = calloc(no_of_edges, sizeof(char));
  CGRAPH_INTEGER i, j;
  arr_make(eid, 0, CGRAPH_INTEGER);

#define FREE_MEMORY() \
  free(vmark); \
  free(emark); \
  arr_free(eid)

  for (i = 0; i < arr_size(vertices); ++i) {
    if (i < 0 || i >= no_of_vertices) {
      FREE_MEMORY();
      CGRAPH_ERROR("Đỉnh không hợp lệ.", CGRAPH_FAILURE);
      return;
    }
    if (!vmark[vertices[i]]) {
      vmark[vertices[i]] = 1;
      atype(CGRAPH_INTEGER) *tmp = cgraph_incident(graph, vertices[i], mode);
      if (!cgraph_err_is_success()) {
        FREE_MEMORY();
        CGRAPH_ERROR("Lỗi lấy danh sách cạnh", CGRAPH_FAILURE);
        return;
      }
      // Đã lấy danh sách cạnh
      for (j = 0; j < arr_size(tmp); ++j) {
        if (!emark[tmp[j]]) {
          emark[tmp[j]] = 1;
          arr_append(eid, tmp[j]);
        }
      }
      arr_free(tmp);
    }
  }

  if (arr_size(eid) > 0) {
    cgraph_delete_edges(graph, eid);
  }
  FREE_MEMORY();

#undef FREE_MEMORY
}
