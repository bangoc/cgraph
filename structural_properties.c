#include "cgraph_constants.h"
#include "cgraph_datatype.h"
#include "cgraph_error.h"
#include "cgraph_interface.h"
#include "cgraph_iqueue.h"
#include "cgraph_ivec.h"
#include "cgraph_paths.h"
#include "cgraph_topology.h"
#include "cgraph_types_internal.h"

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
  cgraph_ivec_t degrees = cgraph_ivec_create(),
                neis = cgraph_ivec_create();
  cgraph_iqueue_t sources = cgraph_iqueue_create();
  CGRAPH_INTEGER node, i, j, nei, vertices_left;

  cgraph_degree_all(graph, &degrees, CGRAPH_OUT, true);

  vertices_left = no_of_nodes;

  /* Kiểm tra xem chúng ta có đỉnh không có cạnh đi ra hay không? */
  for (i = 0; i < no_of_nodes; i++) {
    if (degrees[i] == 0) {
      CGRAPH_CHECK(cgraph_iqueue_enqueue(sources, i));
    }
  }

  /* Xử lý và xóa các đỉnh không có cạnh đi ra */
  while (!cgraph_iqueue_empty(sources)) {
    cgraph_iqueue_poll(sources, &node);

    /* Đánh dấu để bỏ qua trong các lần tìm kiếm tiếp theo */
    degrees[node] = -1;
    vertices_left--;

    /* Lấy các láng giềng đi vào và giảm bậc ra một đơn vị */
    CGRAPH_CHECK(cgraph_neighbors(graph, &neis, node, CGRAPH_IN));
    j = cgraph_ivec_size(neis);
    for (i = 0; i < j; i++) {
      nei = neis[i];
      // if (nei == node) {
      //   continue;
      // }
      degrees[nei]--;
      if (degrees[nei] == 0) {
        CGRAPH_CHECK(cgraph_iqueue_enqueue(sources, nei));
      }
    }
  }

  if (vertices_left < 0) {
    CGRAPH_ERROR("vertices_left < 0 in cgraph_is_dag, possible bug", CGRAPH_FAILURE);
  }

  cgraph_ivec_free(&degrees);
  cgraph_ivec_free(&neis);
  cgraph_iqueue_free(&sources);

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
 * \return Mã lỗi.
 *
 * Độ phức tạp: O(|V|+|E|), trong đó |V| và |E| là số lượng đỉnh và
 * cạnh trong đồ thị đầu vào ban đầu.
 *
 * \sa \ref cgraph_is_dag() nếu bạn chỉ quan tâm đồ thị được cho có phải là DAG hay không, hoặc \ref igraph_feedback_arc_set() để tìm một tập hợp cạnh, sao cho khi loại bỏ thì đồ thị là DAG.
 *
 */
int cgraph_topological_sorting(const cgraph_t graph,
                               cgraph_ivec_t *res,
                               cgraph_neimode_t mode) {
  CGRAPH_INTEGER no_of_nodes = cgraph_vcount(graph);
  cgraph_ivec_t degrees = cgraph_ivec_create(),
                neis = cgraph_ivec_create();
  cgraph_iqueue_t sources = cgraph_iqueue_create();
  cgraph_neimode_t deg_mode;

  if (mode == CGRAPH_ALL || !cgraph_is_directed(graph)) {
    CGRAPH_ERROR("topological sorting does not make sense for undirected graphs", CGRAPH_FAILURE);
  } else if (mode == CGRAPH_OUT) {
    deg_mode = CGRAPH_IN;
  } else if (mode == CGRAPH_IN) {
    deg_mode = CGRAPH_OUT;
  } else {
    CGRAPH_ERROR("invalid mode", CGRAPH_FAILURE);
  }

  /* Không tính đỉnh lặp */
  CGRAPH_CHECK(cgraph_degree_all(graph, &degrees, deg_mode, false));

  cgraph_ivec_setsize(*res, 0);
  /* Chúng ta có đỉnh không có láng giềng đứng trước hay không? */
  for (CGRAPH_INTEGER i = 0; i < no_of_nodes; i++) {
    if (degrees[i] == 0) {
      CGRAPH_CHECK(cgraph_iqueue_enqueue(sources, i));
    }
  }

  /* Lấy và xóa tất cả các đỉnh không có láng giềng đứng trước */
  while (!cgraph_iqueue_empty(sources)) {
    CGRAPH_INTEGER node;
    cgraph_iqueue_poll(sources, &node);
    cgraph_ivec_push_back(res, node);
    degrees[node] = -1;
    CGRAPH_CHECK(cgraph_neighbors(graph, &neis, node, mode));
    for (CGRAPH_INTEGER i = 0; i < cgraph_ivec_size(neis); i++) {
      degrees[neis[i]]--;
      if (degrees[neis[i]] == 0) {
        CGRAPH_CHECK(cgraph_iqueue_enqueue(sources, neis[i]));
      }
    }
  }

  if (cgraph_ivec_size(*res) < no_of_nodes) {
    CGRAPH_ERROR("graph contains a cycle, partial result is returned", CGRAPH_FAILURE);
  }

  cgraph_ivec_free(&degrees);
  cgraph_ivec_free(&neis);
  cgraph_iqueue_free(&sources);
  return 0;
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
 * \return Mã lỗi.
 *
 * Độ phức tạp: O(|E|log|E|+|V|), |V| là số lượng đỉnh,
 * |E| là số lượng cạnh trong đồ thị.
 *
 * \sa \ref cgraph_get_shortest_paths_dijkstra() cho phiên bản với
 * nhiều đỉnh đích hơn.
 **/
int cgraph_get_shortest_path_dijkstra(const cgraph_t graph,
        cgraph_ivec_t *vertices,
        cgraph_ivec_t *edges,
        CGRAPH_INTEGER from,
        CGRAPH_INTEGER to,
        cgraph_rvec_t weights,
        cgraph_neimode_t mode) {
  if (!weights) {
    return cgraph_get_shortest_path(graph, vertices, edges, from, to, mode);
  }
  CGRAPH_INTEGER no_of_nodes = cgraph_vcount(graph);
  CGRAPH_INTEGER no_of_edges = cgraph_ecount(graph);
  cgraph_2wheap_t Q;
  cgraph_rvec_t dists = cgraph_rvec_create();
  if (cgraph_rvec_size(weights) != no_of_edges) {
    CGRAPH_ERROR("Weight vector length does not match", CGRAPH_FAILURE);
  }
  CGRAPH_CHECK(cgraph_2wheap_init(&Q, no_of_nodes));
  cgraph_rvec_init(&dists, no_of_edges);
  for (CGRAPH_INTEGER i = 0; i < no_of_nodes; ++i) {
    dists[i] = -1.0;
  }
  CGRAPH_INTEGER *parents =
    (CGRAPH_INTEGER *)malloc(sizeof(CGRAPH_INTEGER) * no_of_nodes);
  dists[from] = 0.0;
  parents[from] = -1;
  cgraph_2wheap_push_with_index(&Q, from, 0);
  cgraph_ivec_t neis = cgraph_ivec_create();
  bool found = false;
  while (!cgraph_2wheap_empty(&Q) && !found) {
    CGRAPH_INTEGER nlen, minnei = cgraph_2wheap_max_index(&Q);

    /* Hack đảo dấu để tránh giá trị vô cùng lớn */
    CGRAPH_REAL mindist = -cgraph_2wheap_delete_max(&Q);
    if (minnei == to) {
      found = true;
      break;
    }
    cgraph_incident(graph, &neis, minnei, mode);
    nlen = cgraph_ivec_size(neis);
    for (CGRAPH_INTEGER i = 0; i < nlen; ++i) {
      CGRAPH_INTEGER edge = neis[i];
      CGRAPH_INTEGER tto = CGRAPH_OTHER(graph, edge, minnei);
      CGRAPH_REAL altdist = mindist + weights[edge];
      CGRAPH_REAL curdist = dists[tto];
      if (curdist < 0) {
        dists[tto] = altdist;
        parents[tto] = edge;
        CGRAPH_CHECK(cgraph_2wheap_push_with_index(&Q, tto, -altdist));
      } else if (altdist < curdist) {
        dists[tto] = altdist;
        parents[tto] = edge;
        CGRAPH_CHECK(cgraph_2wheap_modify(&Q, tto, -altdist));
      }
    }
  }
  if (!found) {
    CGRAPH_ERROR("Path not found", CGRAPH_FAILURE);
    if (vertices) {
      cgraph_ivec_setsize(*vertices, 0);
    }
    if (edges) {
      cgraph_ivec_setsize(*edges, 0);
    }
    return 0;
  }

  if (vertices || edges) {
    cgraph_ivec_t vvec, evec;
    CGRAPH_INTEGER act = to;
    CGRAPH_INTEGER size = 0;
    CGRAPH_INTEGER edge;
    while (parents[act] != -1) {
      ++size;
      edge = parents[act];
      act = CGRAPH_OTHER(graph, edge, act);
    }
    if (vertices) {
      CGRAPH_CHECK(cgraph_ivec_init(vertices, size + 1));
      vvec = *vertices;
      vvec[size] = to;
    }
    if (edges) {
      CGRAPH_CHECK(cgraph_ivec_init(edges, size));
      evec = *edges;
    }
    act = to;
    while (parents[act] != -1) {
      edge = parents[act];
      act = CGRAPH_OTHER(graph, edge, act);
      --size;
      if (vertices) {
        vvec[size] = act;
      }
      if (edges) {
        evec[size] = edge;
      }
    }
  }
  cgraph_2wheap_free(&Q);
  cgraph_rvec_free(&dists);
  free(parents);
  return 0;
}

/**
 * \ref igraph_get_shortest_path
 **/
int cgraph_get_shortest_path(const cgraph_t graph,
        cgraph_ivec_t *vertices,
        cgraph_ivec_t *edges,
        CGRAPH_INTEGER from,
        CGRAPH_INTEGER to,
        cgraph_neimode_t mode) {
  CGRAPH_INTEGER no_of_edges = cgraph_ecount(graph);
  cgraph_rvec_t weights = cgraph_rvec_create();
  cgraph_rvec_init(&weights, no_of_edges);
  for (CGRAPH_INTEGER i = 0; i < no_of_edges; ++i) {
    weights[i] = 1.0;
  }
  CGRAPH_INTEGER ret = cgraph_get_shortest_path_dijkstra(graph, vertices, edges, from, to, weights, mode);
  cgraph_rvec_free(&weights);
  return ret;
}
