#include "base/cgraph_arr.h"
#include "base/cgraph_constants.h"
#include "base/cgraph_datatype.h"
#include "base/cgraph_error.h"
#include "cgraph_interface.h"
#include "cgraph_paths.h"
#include "cgraph_topology.h"
#include "cgen/all.h"

#include <stdlib.h>

/**
 * \function cgraph_is_dag
 * \ref igraph_is_dag
 * Kiểm tra liệu một đồ thị có hướng có là DAG hay không.
 *
 * </para><para>
 * Một đồ thị có hướng không chứa chu trình được gọi là DAG
 * (Directed Acyclic Graph).
 *
 * \param graph Đồ thị đầu vào.
 * \return true nếu đồ thị là DAG, false nếu ngược lại.
 *
 * Độ phức tạp: O(|V|+|E|), trong đó |V| và |E| là số lượng đỉnh và
 * số lượng cạnh trong đồ thị ban đầu.
 *
 * \sa \ref cgraph_topological_sorting() để lấy một trật tự topo nếu
 * có của một đồ thị có hướng.
 */
bool cgraph_is_dag(const cgraph_t graph) {
  if (!cgraph_is_directed(graph)) {
    return false;
  }

  CGRAPH_INTEGER no_of_nodes = cgraph_vcount(graph);
  struct gsllist *sources = gsl_create_list(NULL);
  CGRAPH_INTEGER node, i, j, nei, vertices_left;

  atype(CGRAPH_INTEGER) *degrees = cgraph_degree_all(graph, CGRAPH_OUT, true);

  vertices_left = no_of_nodes;

  /* Kiểm tra xem chúng ta có đỉnh không có cạnh đi ra hay không? */
  for (i = 0; i < no_of_nodes; i++) {
    if (degrees[i] == 0) {
      que_enq(sources, gtype_l(i));
    }
  }

  /* Xử lý và xóa các đỉnh không có cạnh đi ra */
  while (!que_is_empty(sources)) {
    node = que_peek(sources).l;
    que_deq(sources);

    /* Đánh dấu để bỏ qua trong các lần tìm kiếm tiếp theo */
    degrees[node] = -1;
    vertices_left--;

    /* Lấy các láng giềng đi vào và giảm bậc ra 1 đơn vị */
    atype(CGRAPH_INTEGER) *neis = cgraph_neighbors(graph, node, CGRAPH_IN);
    j = arr_size(neis);
    for (i = 0; i < j; i++) {
      nei = neis[i];
      // if (nei == node) {
      //   continue;
      // }
      degrees[nei]--;
      if (degrees[nei] == 0) {
        que_enq(sources, gtype_l(nei));
      }
    }
    arr_free(neis);
  }

  arr_free(degrees);
  gsl_free(sources);

  return (vertices_left == 0);
}

/**
 * \function cgraph_topological_sorting
 * \ref igraph_topological_sorting
 * \brief Sắp xếp các đỉnh của một đồ thị theo thứ tự topo.
 *
 * </para><para>
 * Trật tự topo của các đỉnh của một đồ thị có hướng không chứa chu
 * trình (DAG) là một trật tự tuyến tính trong đó tất cả các đỉnh đều
 * xuất hiện trước các đỉnh có cạnh nối với nó. Tất cả các DAG đều có
 * ít nhất một trật tự topo, và có thể có nhiều trật tự topo khác
 * nhau. Hàm này trả về một trật tự topo nếu có của đồ thị. Nếu đồ
 * thị có chứa ít nhất một chu trình, một phần của trật tự topo được
 * trả về và một cảnh báo được đưa ra.
 *
 * \param graph Đồ thị đầu vào.
 * \param res Con trỏ tới một vec-tơ, danh sách đỉnh được sắp xếp
 * theo trật tự topo sẽ được lưu ở đây, vec-tơ sẽ được thay đổi kích
 * thước nếu cần.
 * \param mode Tùy chỉnh hướng cho các cạnh.
 * Đối với \c CGRAPH_OUT, trật tự đảm bảo rằng tất cả các đỉnh đều
 * xuất hiện trước các đỉnh mà nó có cạnh đi tới đó, như vậy các đỉnh
 * không có cạnh đi tới sẽ xuất hiện đầu tiên. Đối với \c CGRAPH_IN,
 * quan hệ gần như ngược lại: Mỗi đỉnh đều xuất hiện trước các nút có
 * cạnh đi tới nó. Các đỉnh không có cạnh đi tới xuất hiện đầu tiên.
 * \return Mảng chứa các đỉnh theo thứ tự topo.
 *
 * Độ phức tạp: O(|V|+|E|), trong đó |V| và |E| là số lượng đỉnh và
 * cạnh trong đồ thị đầu vào ban đầu.
 *
 * \sa \ref cgraph_is_dag() nếu bạn chỉ quan tâm đồ thị được cho có phải là DAG hay không, hoặc \ref igraph_feedback_arc_set() để tìm một tập hợp cạnh, sao cho khi loại bỏ thì đồ thị là DAG.
 *
 */
atype(CGRAPH_INTEGER) *cgraph_topological_sorting(const cgraph_t graph,
                               cgraph_neimode_t mode) {
  cgraph_err_reset();
  CGRAPH_INTEGER no_of_nodes = cgraph_vcount(graph);
  cgraph_neimode_t deg_mode;

  if (mode == CGRAPH_ALL || !cgraph_is_directed(graph)) {
    CGRAPH_ERROR("Sắp xếp topo vô nghĩa đối với đồ thị"
                 " vô hướng", CGRAPH_FAILURE);
    return NULL;
  } else if (mode == CGRAPH_OUT) {
    deg_mode = CGRAPH_IN;
  } else if (mode == CGRAPH_IN) {
    deg_mode = CGRAPH_OUT;
  } else {
    CGRAPH_ERROR("Tham số mode không hợp lệ", CGRAPH_FAILURE);
    return NULL;
  }
  struct gsllist *sources = gsl_create_list(NULL);

  /* Không tính đỉnh lặp */
  atype(CGRAPH_INTEGER) *degrees = cgraph_degree_all(graph, deg_mode, false);
  /* Chúng ta có đỉnh không có láng giềng đứng trước hay không? */
  for (CGRAPH_INTEGER i = 0; i < no_of_nodes; i++) {
    if (degrees[i] == 0) {
      que_enq(sources, gtype_l(i));
    }
  }

  arr_make(res, 0, CGRAPH_INTEGER);

  /* Lấy và xóa tất cả các đỉnh không có láng giềng đứng trước */
  while (!que_is_empty(sources)) {
    CGRAPH_INTEGER node;
    node = que_peek(sources).l;
    que_deq(sources);
    arr_append(res, node);
    degrees[node] = -1;
    atype(CGRAPH_INTEGER) *neis = cgraph_neighbors(graph, node, mode);
    for (CGRAPH_INTEGER i = 0; i < arr_size(neis); i++) {
      degrees[neis[i]]--;
      if (degrees[neis[i]] == 0) {
        que_enq(sources, gtype_l(neis[i]));
      }
    }
    arr_free(neis);
  }

  arr_free(degrees);
  gsl_free(sources);

  if (arr_size(res) < no_of_nodes) {
    CGRAPH_WARNING("Đồ thị chứa chu trình, một phần kết quả được trả về");
  }
  return res;
}

/**
 * \ingroup cấu trúc
 * \function cgraph_get_shortest_paths
 * \brief Tìm các đường đi ngắn nhất từ/tới một đỉnh.
 *
 * </para><para>
 * Nếu có nhiều hơn một đường đi giữa hai đỉnh hàm này chỉ trả về một
 * \param graph Đối tượng đồ thị.
 * \param vertices Kết quả, mã số của các đỉnh trên các đường đi.
 *        Đây là một vec-tơ của các vec-tơ số nguyên, mỗi phần tử là
 *        một vec-tơ số nguyên. Các vec-tơ này cần được khởi tạo
 *        trước khi truyền vào hàm. Các vec-tơ này sẽ được thay đổi
 *        kích thước nếu cần và nạp đầy với các chỉ số đỉnh trên các
 *        đường đi từ/tới các đỉnh. Truyền một con trỏ NULL nếu bạn
 *        không cần những vec-tơ này.
 * \param edges Kết quả, các chỉ số cạnh trên các đường đi. Đây là
 *        một vec-tơ của các vec-tơ, mỗi phần tử là một vec-tơ số
 *        nguyên. Các vec-tơ này phải được khởi tạo trước khi truyền
 *        cho hàm, các vec-tơ sẽ được thay đổi kích thước nếu cần và
 *        được lấp đầy với các chỉ số đỉnh trên các đường đi từ/tới
 *        các đỉnh. Truyền một con trỏ NULL nếu bạn không cần những
 *        vec-tơ này.
 * \param from Chỉ số của đỉnh từ/tới đó chúng ta cần phải tính đường
 *        đi ngắn nhất.
 * \param to Danh sách các chỉ số đỉnh từ/tới đó các đường đi ngắn
 *        nhất sẽ được tính. Một đỉnh có thể được đưa vào nhiều lần.
 * \param mode Tham số điều chỉnh hướng trong tính đường đi ngắn nhất.
 *        Các giá trị có thể có:
 *        \clist
 *        \cli CGRAPH_OUT
 *          Đường đi ra được tính.
 *        \cli CGRAPH_IN
 *          Các đường đi tới được tính.
 *        \cli CGRAPH_ALL
 *          Đồ thị có hướng được coi như đồ thị vô hướng trong tính
 *          toán.
 *        \endclist
 * \param predecessors Con trỏ tới một vec-tơ đã được khởi tạo hoặc
 *        NULL. Nếu khác NULL, vec-tơ này sẽ chứa các đỉnh đứng trước
 *        của mỗi đỉnh trong cây đường đi ngắn nhất một nguồn được
 *        trả về. Đỉnh trước của một đỉnh i trong cây là đỉnh từ đó đi
 *        tới i. Đỉnh trước của đỉnh bắt đầu (tham số \c from) là
 *        chính nó theo định nghĩa. Đỉnh trước là -1 nghĩa là không
 *        đi tới được đỉnh đó từ đỉnh bắt đầu. Quá trình tìm kiếm kết
 *        thúc nếu đã tìm thấy tất cả các đỉnh trong \c to.
 * \param inbound_edges Con trỏ tới một vec-tơ số nguyên đã được
 *        khởi tạo hoặc NULL. Nếu khác NULL, vec-tơ này sẽ chứa các
 *        cạnh vào của các đỉnh trong cây đường đi ngắn nhất một
 *        nguồn được trả về. Cạnh vào của đỉnh i trong cây là cạnh mà
 *        đỉnh i được tìm thấy qua đó. Đỉnh bắt đầu và các đỉnh không
 *        được tìm thấy sẽ có giá trị -1 ở các vị trí tương ứng trong
 *        vec-tơ. Tiến trình tìm kiếm kết thúc nếu tất cả các đỉnh
 *        trong \c to được tìm thấy.
 *
 * \return Trả về con trỏ tới đối tượng paths ::struct paths *
 *
 * Độ phức tạp: O(|V|+|E|),
 * |V| Số lượng đỉnh,
 * |E| Số lượng cạnh trong đồ thị.
 *
 * \sa \ref cgraph_shortest_paths() Nếu bạn chỉ cần độ dài cạnh chứ
 * không cần các cạnh.
 *
 */
struct paths *cgraph_get_shortest_paths(const cgraph_t graph,
                              CGRAPH_INTEGER from,
                              atype(CGRAPH_INTEGER) *to,
                              cgraph_neimode_t mode) {
  cgraph_err_reset();
  CGRAPH_INTEGER no_of_nodes = cgraph_vcount(graph);
  CGRAPH_INTEGER *father;

  CGRAPH_INTEGER to_reach;
  CGRAPH_INTEGER reached = 0;

  if (from < 0 || from >= no_of_nodes) {
    CGRAPH_ERROR("Chỉ số đỉnh from không hợp lệ", CGRAPH_FAILURE);
    return NULL;
  }
  if (mode != CGRAPH_OUT && mode != CGRAPH_IN && mode != CGRAPH_ALL) {
    CGRAPH_ERROR("Tham số mode không hợp lệ", CGRAPH_FAILURE);
    return NULL;
  }

  paths_make(res, arr_size(to), no_of_nodes);
  father = calloc(no_of_nodes, sizeof(CGRAPH_INTEGER));
  if (father == 0) {
    CGRAPH_ERROR("Lỗi cấp phát bộ nhớ", CGRAPH_FAILURE);
    return NULL;
  }
  struct gsllist *q = gsl_create_list(NULL);

  /* Đánh dấu các đỉnh cần đi tới */
  to_reach = arr_size(to);
  for (int i = 0; i < arr_size(to); ++i) {
    if (father[ to[i] ] == 0) {
      father[ to[i] ] = -1;
    } else {
      // Đỉnh này được đưa vào nhiều lần
      to_reach--;
    }
  }

  /* Ý nghĩa của father[i]:
   *
   * - Nếu father[i] < 0, nghĩa là cần tìm đường đi tới đỉnh i
   * nhưng vẫn chưa tìm thấy đường đi tới i.
   *
   * - Nếu father[i] = 0, nghĩa là không cần tìm đường đi tới i và
   * vẫn chưa đi tới i.
   *
   * - Nếu father[i] = 1, nghĩa là i là đỉnh bắt đầu.
   *
   * - Trong các trường hợp khác father[i] là chỉ số của cạnh đi
   * tới i cộng 2.
   */

  que_enq(q, gtype_l(from + 1));
  /*
    Trường hợp from có trong to
  */
  if (father[from] < 0) {
    reached++;
  }
  father[from] = 1;

  while (!que_is_empty(q) && reached < to_reach) {
    CGRAPH_INTEGER act = que_peek(q).l;
    que_deq(q);

    // Các giá trị trong hàng đợi là mã đỉnh + 1
    --act;

    atype(CGRAPH_INTEGER) *tmp = cgraph_incident(graph, act, mode);
    for (int j = 0; j < arr_size(tmp); j++) {
      CGRAPH_INTEGER edge = tmp[j];
      CGRAPH_INTEGER neighbor = CGRAPH_OTHER(graph, edge, act);
      if (father[neighbor] > 0) {
        continue;
      } else if (father[neighbor] < 0) {
        reached++;
      }
      father[neighbor] = edge + 2;
      que_enq(q, gtype_l(neighbor + 1));
    }
    arr_free(tmp);
  }

  if (reached < to_reach) {
    CGRAPH_WARNING("Không thể đi tới một số đỉnh");
  }

  /* Xuất thông tin `predecessors' */
  for (CGRAPH_INTEGER i = 0; i < no_of_nodes; i++) {
    if (father[i] <= 0) {
      /* Chưa đi tới i */
      (res->predecessors)[i] = -1;
    } else if (father[i] == 1) {
      /* là đỉnh bắt đầu */
      (res->predecessors)[i] = i;
    } else {
      /* Đã đi tới i qua canh có ID = father[i] - 2 */
      (res->predecessors)[i] = CGRAPH_OTHER(graph, father[i] - 2, i);
    }
  }

  /* Xuất thông tin `inbound_edges' */
  for (int i = 0; i < no_of_nodes; i++) {
    if (father[i] <= 1) {
      /* Chưa đi tới i hoặc i là đỉnh bắt đầu */
      (res->inbound_edges)[i] = -1;
    } else {
      /* Đã đi tới i qua cạnh có ID = father[i] - 2 */
      (res->inbound_edges)[i] = father[i] - 2;
    }
  }

  /* Xuất thông tin `vertices' và `edges' */
  for (int i = 0; i < arr_size(to); ++i) {
    CGRAPH_INTEGER node = to[i];
    (res->reached)[i] = (father[node] >= 0);
    if (father[node] > 0) {
      CGRAPH_INTEGER act = node;
      CGRAPH_INTEGER size = 0;
      CGRAPH_INTEGER edge;
      while (father[act] > 1) {
        size++;
        edge = father[act] - 2;
        act = CGRAPH_OTHER(graph, edge, act);
      }
      arr_resize(res->vertices[i], size + 1);
      res->vertices[i][size] = node;
      arr_resize(res->edges[i], size);
      act = node;
      while (father[act] > 1) {
        size--;
        edge = father[act] - 2;
        act = CGRAPH_OTHER(graph, edge, act);
        res->vertices[i][size] = act;
        res->edges[i][size] = edge;
      }
    }
  }

  free(father);
  gsl_free(q);

  return res;
}

/**
 * \ingroup Cấu trúc
 * \function cgraph_get_shortest_paths_dijkstra
 * \brief Tính các đường đi ngắn nhất có trọng số từ/tới một đỉnh
 *
 * </para><para>
 * Nếu có nhiều hơn một đường đi ngắn nhất giữa hai đỉnh, hàm này chỉ trả về một đường đi.
 * \param graph Đối tượng đồ thị.
 * \param vertices Kết quả, chỉ số của các đỉnh trên các đường đi.
 *        Đây là một vec-tơ của các vec-tơ, mỗi phần tử là một vec-tơ
 * số nguyên. Các vec-tơ cần được khởi tạo trước khi truyền cho hàm.
 * Các vec-tơ sẽ được xóa và/hoặc thay đổi kích thước và được lấp đầy
 * bằng các chỉ số của các đỉnh trên các đường đi từ/tới các đỉnh.
 * Cung cấp một con trỏ NULL ở vị trí này nếu bạn không cần các
 * vec-tơ này. Thông thường tham số này, hoặc edges là khác NULL,
 * tuy nhiên sẽ không có thông báo lỗi nếu cả hai đều là các con trỏ
 * NULL.
 * \param edges Kết quả, các chỉ số của các cạnh trên các đường đi.
 *        Đây là một vec-tơ của các vec-tơ, mỗi phần tử là một vec-tơ. Các vec-tơ này cần được khởi tạo trước khi được truyền cho hàm. Trong các xử lý, các vec-tơ sẽ được xóa và/hoặc thay đổi kích thước hợp lý và được lấp đầy với các chỉ số của các cạnh trên các đường đi từ/tới các cạnh. Truyền một con trỏ NULL nếu bạn không cần các chỉ số này. Thông thường tham số này hoặc vertices khác NULL, tuy nhiên sẽ không có thông báo lỗi nào nếu cả hai đều là các con trỏ NULL.
 * \param from Chỉ số của đỉnh mà các đường đi từ/tới đó sẽ được tính.
 * \param to Danh sách các chỉ số đỉnh tới/từ đó các đường đi ngắn nhất sẽ được tính. Một đỉnh có thể được đưa vào nhiều lần.
 * \param weights Một vec-tơ chứa các trọng số cạnh, tất cả các trọng số cạnh phải dương.
 * \param mode Kiểu đường dẫn ngắn nhất được sử dụng cho các tính toán trong các đồ thị có hướng. Các giá trị có thể:
 *        \clist
 *        \cli CGRAPH_OUT
 *          Các đường đi ra được tính.
 *        \cli CGRAPH_IN
 *          Các đường đi vào được tính.
 *        \cli CGRAPH_ALL
 *          đồ thị có hướng được coi như vô hướng trong tính toán.
 *        \endclist
 * \param predecessors Một con trỏ tới vec-tơ số nguyên hoặc NULL.
 * Nếu khác NULL, vec-tơ này sẽ chứa các đỉnh trước của mỗi đỉnh trong
 * cây đường đi ngắn nhất từ một nguồn được trả về. Đỉnh trước (
 * predecessor) của một đỉnh i trong cây là đỉnh mà từ đó đỉnh i được
 * tiếp cận. Đỉnh trước của đỉnh bắt đầu (tham số \c from) là chính nó
 * theo định nghĩa. Đỉnh trước của một đỉnh bằng -1 nghĩa là đỉnh đó
 * không được phát hiện trong quá trình tìm kiếm từ đỉnh nguồn. Lưu ý
 * tiến trình tìm kiếm sẽ dừng lại khi nếu tất cả các đỉnh trong \c
 * to đã được tìm thấy.
 * \param inbound_edges Một vec-tơ số nguyên đã được khởi tạo hoặc
 * NULL. Nếu khác NULL vec-tơ này sẽ chứa chỉ số các cạnh trong của
 * mỗi đỉnhtrong cây đường đi ngắn nhất từ một đỉnh. Cạnh trong của
 * một đỉnh i trong cây là cạnh mà thông qua đó đỉnh i được tiếp cận.
 * Đỉnh bắt đầu và các đỉnh không được tìm thấy sẽ có giá trị -1 ở các
 * vị trí tương ứng trong vec-tơ. Lưu ý tiến trình tìm kiếm sẽ dừng
 * lại khi tất cả các đường đi ngắn nhất tới tất cả các đỉnh trong \c
 * to đều đã được tìm thấy.
 * \return Trả về con trỏ tới đối tượng paths ::struct paths *
 *
 * Độ phức tạp: O(|E|log|E|+|V|), trong đó |V| là số lượng đỉnh và
 * |E| là số lượng cạnh.
 *
 * \sa \ref cgraph_get_shortest_paths() nếu trọng số của tất cả các
 * cạnh đều bằng nhau.
 *
 * \example examples/simple/igraph_get_shortest_paths_dijkstra.c
 */
struct paths *cgraph_get_shortest_paths_dijkstra(const cgraph_t graph,
                          CGRAPH_INTEGER from,
                          atype(CGRAPH_INTEGER) *to,
                          atype(CGRAPH_REAL) *weights,
                          cgraph_neimode_t mode) {
    /* Chi tiết triển khai. Đây là triển khai cơ bản cho giải thuật
    Dijkstra với một heap nhị phân. Heap được đánh chỉ mục, nghĩa là,
    nó không chỉ chứa các khoảng cách, mà còn chứa các đỉnh gắn với
    các giá trị khoảng cách đó.

       Các thủ thuật:
       - Các giá trị khoảng cách được đảo dấu trước khi được đưa vào
       heap, như vậy chúng ta sẽ sử dụng heap cực đại thay cho heap
       cực tiểu.
       - Chúng ta không sử dụng CGRAPH_INFINITY trong quá trình tính
       toán trong vec-tơ khoảng cách, bởi vì CGRAPH_FINITE() có thể
       kéo theo gọi hàm và chúng ta không muốn điều đó. Vì vậy chúng
       ta lưu khoảng cách + 1.0 thay cho khoảng cách, và giá trị 0
       tương đương với giá trị vô cùng.
       - `parents' gán tất cả các chỉ số cạnh trong của tất cả các
       đỉnh trong cây đường đi ngắn nhất cho các đỉnh. Trong triển
       khai này chỉ số cạnh + 1 được lưu, 0 nghĩa là đỉnh không được
       phát hiện.
    */
  cgraph_err_reset();
  if (!weights) {
    return cgraph_get_shortest_paths(graph, from, to, mode);
  }

  CGRAPH_INTEGER no_of_nodes = cgraph_vcount(graph);
  CGRAPH_INTEGER no_of_edges = cgraph_ecount(graph);

  if (arr_size(weights) != no_of_edges) {
    CGRAPH_ERROR("Lỗi độ dài vec-tơ trọng số", CGRAPH_FAILURE);
    return NULL;
  }
  for (long i = 0; i < no_of_edges; ++i) {
    if (weights[i] < 0) {
      CGRAPH_ERROR("Lỗi trọng số âm", CGRAPH_FAILURE);
      return NULL;
    }
  }
  struct p2ways *q = p2w_create(gtype_cmp_d);
  arr_make(dists, no_of_nodes, CGRAPH_REAL);
  CGRAPH_INTEGER i, to_reach;
  paths_make(res, arr_size(to), no_of_nodes);
  arr_fill(dists, -1.0);

  CGRAPH_INTEGER *parents = calloc(no_of_nodes, sizeof(CGRAPH_INTEGER));
  if (parents == 0) {
    CGRAPH_ERROR("Lỗi cấp phát bộ nhớ", CGRAPH_FAILURE);
    return NULL;
  }

  bool *is_target = calloc(no_of_nodes, sizeof(bool));
  if (is_target == 0) {
    CGRAPH_ERROR("Lỗi cấp phát bộ nhớ", CGRAPH_FAILURE);
    return NULL;
  }

  // Đánh dấu các đỉnh cần tới
  to_reach = arr_size(to);
  for (CGRAPH_INTEGER i = 0; i < arr_size(to); ++i) {
    if (!is_target[ to[i] ]) {
      is_target[ to[i] ] = true;
    } else {
      to_reach--;  /* Đỉnh xuất hiện nhiều lần trong to */
    }
  }

  dists[from] = 0.0;
  parents[from] = 0;
  p2w_push_with_index(q, from, gtype_d(0.0));

  while (!p2w_is_empty(q) && to_reach > 0) {
    CGRAPH_INTEGER nlen, minnei = p2w_max_index(q);
    CGRAPH_REAL mindist = -p2w_delete_max(q).d;

    if (is_target[minnei]) {
      is_target[minnei] = false;
      to_reach--;
    }

    // Cập nhật đường đi ngắn nhất cho các láng giềng của 'minnei'
    atype(CGRAPH_INTEGER) *neis = cgraph_incident(graph, minnei, mode);
    nlen = arr_size(neis);
    for (CGRAPH_INTEGER i = 0; i < nlen; i++) {
      CGRAPH_INTEGER edge = neis[i];
      CGRAPH_INTEGER tto = CGRAPH_OTHER(graph, edge, minnei);
      CGRAPH_REAL altdist = mindist + weights[edge];
      CGRAPH_REAL curdist = dists[tto];
      if (curdist < 0 || altdist < curdist) {
        // Khoảng cách hữu hạn đầu tiên hoặc một đường đi ngắn hơn.
        dists[tto] = altdist;
        parents[tto] = edge + 1;
        CGRAPH_CHECK(
          p2w_push_with_index(q, tto, gtype_d(-altdist)));
      }
    }
    arr_free(neis);
  } /* !igraph_2wheap_empty(q) */

  if (to_reach > 0) {
    CGRAPH_WARNING("Có đỉnh không thể đi tới");
  }

  // Xuất dữ liệu `predecessors'
  for (i = 0; i < no_of_nodes; i++) {
    if (i == from) {
      /* i là đỉnh bắt đầu */
      res->predecessors[i] = i;
    } else if (parents[i] <= 0) {
      // Chưa đi tới i
      res->predecessors[i] = -1;
    } else {
      // Đi tới i qua cạnh với chỉ số = parents[i] - 1
      res->predecessors[i] = CGRAPH_OTHER(graph, parents[i] - 1, i);
    }
  }

  // Xuất dữ liệu `inbound_edges'
  for (i = 0; i < no_of_nodes; i++) {
    if (parents[i] <= 0) {
      // Chưa đi tới i
      res->inbound_edges[i] = -1;
    } else {
      // Đi tới i qua cạnh với chỉ số = parents[i] - 1
      res->inbound_edges[i] = parents[i] - 1;
    }
  }

  // Các đường đi ngắn nhất dựa trên các chỉ số đỉnh và/hoặc cạnh
  for (CGRAPH_INTEGER i = 0; i < arr_size(to); i++) {
    CGRAPH_INTEGER node = to[i];
    CGRAPH_INTEGER size, act, edge;
    size = 0;
    act = node;
    (res->reached)[i] = (parents[node] > 0 || node == from);
    while (parents[act]) {
      size++;
      edge = parents[act] - 1;
      act = CGRAPH_OTHER(graph, edge, act);
    }
    arr_resize(res->vertices[i], size + 1);
    res->vertices[i][size] = node;
    arr_resize(res->edges[i], size);
    act = node;
    while (parents[act]) {
      edge = parents[act] - 1;
      act = CGRAPH_OTHER(graph, edge, act);
      size--;
      res->vertices[i][size] = act;
      res->edges[i][size] = edge;
    }
  }

  p2w_free(q);
  arr_free(dists);
  free(is_target);
  free(parents);

  return res;
}

/**
 * \function cgraph_get_shortest_path_dijkstra
 * \ref igraph_get_shortest_path_dijkstra
 * Đường đi ngắn nhất có trọng số từ một đỉnh tới một đỉnh khác.
 *
 * Tìm một đường đi ngắn nhất có trọng số (dương) từ một đỉnh tới một
 * đỉnh khác, sử dụng giải thuật Dijkstra.
 *
 * </para><para>Hàm này là một trường hợp đặc biệt (và là bao đóng)
 * của
 * \ref cgraph_get_shortest_paths_dijkstra().
 *
 * \param graph Đồ thị đầu vào, có thể có hướng hoặc vô hướng.
 * \param vertices Con trỏ tới một vec-tơ đã được khởi tạo, hoặc con
 * trỏ NULL. Nếu nó là một con trỏ NULL, thì các chỉ số đỉnh trên
 * đường dẫn được lưu ở đây, bao gồm các đỉnh nguồn và đích.
 * \param edges Con trỏ tới một vec-tơ chưa được khởi tạo hoặc NULL.
 * Nếu không phải con trỏ NULL thì các chỉ số cạnh trên đường dẫn
 * được lưu ở đây.
 * \param from Chỉ số của đỉnh nguồn.
 * \param to Chỉ số của đỉnh đích.
 * \param weights Vec-tơ trọng số cạnh cùng thứ tự với các cạnh.
 * Các trọng số phải không âm, nếu ngược lại giải thuật sẽ hoạt
 * động sai.
 * \param mode Một hằng số xác định cách sử dụng hướng của cạnh
 * trong đồ thị có hướng. \c CGRAPH_OUT đi theo hướng của cạnh, \c
 * CGRAPH_IN đi theo chiều ngược lại, và \c CGRAPH_ALL bỏ qua hướng
 * của cạnh. Tham số này bị bỏ qua đối với đồ thị vô hướng.
 * \return Trả về con trỏ tới đối tượng path ::struct path *
 *
 * Độ phức tạp: O(|E|log|E|+|V|), |V| là số lượng đỉnh,
 * |E| là số lượng cạnh trong đồ thị.
 *
 * \sa \ref cgraph_get_shortest_paths_dijkstra() cho phiên bản với
 * nhiều đỉnh đích hơn.
 **/
struct path *cgraph_get_shortest_path_dijkstra(const cgraph_t graph,
        CGRAPH_INTEGER from,
        CGRAPH_INTEGER to,
        atype(CGRAPH_REAL) *weights,
        cgraph_neimode_t mode) {
  arr_make(pto, 0, CGRAPH_INTEGER);
  arr_append(pto, to);
  struct paths *tmp = cgraph_get_shortest_paths_dijkstra(graph,
                        from, pto, weights, mode);
  if (!tmp) {
    CGRAPH_ERROR("Lỗi tìm đường", CGRAPH_FAILURE);
    return NULL;
  }
  path_make(res);
  arr_swap(res->vertices, tmp->vertices[0]);
  arr_swap(res->edges, tmp->edges[0]);
  res->reached = tmp->reached[0];
  arr_free(pto);
  paths_free(tmp);
  return res;
}

/**
 * \ref igraph_get_shortest_path
 * Tìm đường đi ngắn nhất từ đỉnh from tới đỉnh to
 *
 * @param graph Con trỏ tới đối tượng đồ thị
 * @param from Chỉ số đỉnh nguồn
 * @param to Chỉ số đỉnh đích
 * @mode Chế độ
 * @return Đường đi theo đỉnh và đường đi theo cạnh trong
 *   1 đối tượng ::struct path
 **/
struct path *cgraph_get_shortest_path(const cgraph_t graph,
        const CGRAPH_INTEGER from,
        const CGRAPH_INTEGER to,
        const cgraph_neimode_t mode) {
  cgraph_err_reset();
  arr_make(pto, 0, CGRAPH_INTEGER);
  arr_append(pto, to);
  struct paths *tmp =
      cgraph_get_shortest_paths(graph, from, pto, mode);
  if (!tmp) {
    CGRAPH_ERROR("Lỗi tìm đường đi giữa các đỉnh", CGRAPH_FAILURE);
    return NULL;
  }
  path_make(res);
  arr_swap(res->vertices, tmp->vertices[0]);
  arr_swap(res->edges, tmp->edges[0]);
  res->reached = tmp->reached[0];
  arr_free(pto);
  paths_free(tmp);
  return res;
}
