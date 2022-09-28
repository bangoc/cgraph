#ifndef BASE_CGRAPH_DATATYPE_H_
#define BASE_CGRAPH_DATATYPE_H_

#include "cgen/vec/arr.h"
#include "base/cgraph_basic_types.h"

/**
 * \brief Cấu trúc dữ liệu biểu diễn đồ thị
 * \tham khảo: igraph_t
 *
 * Cấu trúc chỉ mục 2 tầng cho biểu diễn đồ thị dạng danh sách cạnh.
 * Nó có các thành phần sau:
 * - <b>n</b> Số lượng đỉnh, các đỉnh được quy ước là số nguyên không
 * âm có giá trị từ 0 đến n-1.
 * - <b>directed</b> Đồ thị có hướng (directed == true) hoặc vô
 * hướng (directed == false).
 * - <b>from</b> Cột đầu tiên của danh sách cạnh.
 * - <b>to</b> Cột thứ hai của danh sách cạnh.
 * - <b>oi</b> Chỉ số của cạnh được sắp xếp tăng dần theo cột thứ
 * nhất rồi đến cột thứ hai. Cạnh đầu tiên theo thứ tự này là
 * from[oi[0]], to[oi[0]], trong đó from[oi[0]] là nhỏ nhất trong tất
 * cả các giá trị from (sắp xếp chỉ số). Độ dài vec-tơ này bằng số
 * lượng cạnh có trong đồ thị.
 * - <b>ii</b> Chỉ số của cạnh được sắp xếp tăng dần theo cột thứ hai
 * rồi đến cột thứ nhất. Cạnh đầu tiên theo thứ tự này là
 * from[ii[0]] , to[ii[0]], trong đó to[ii[0]] có giá trị nhỏ nhất
 * trong tất cả các giá trị ii (sắp xếp chỉ số). Độ dài vec-tơ này
 * bằng số lượng cạnh có trong đồ thị.
 * - <b>os</b> Chứa các con trỏ tới danh sách cạnh đi ra từ tất cả
 * các đỉnh. Cạnh đầu tiên \em từ đỉnh v là cạnh \c from[oi[os[v]]],
 * to [oi[os[v]]] nếu \c os[v] < os[v+1]. Nếu \c os[v] == os[v+1] thì
 * không có cạnh đi ra từ đỉnh v. Độ dài của vec-tơ os bằng số lượng
 * đỉnh cộng 1, phần tử cuối cùng luôn bằng số lượng cạnh và được sử
 * dụng để tra cứu các cạnh cho đỉnh n-1.
 * - <b>is</b> Về cơ bản tương tự như <b>os</b>, nhưng đối với
 * các cạnh đi vào (vec-tơ ii).
 *
 * Đối với đồ thị vô hướng, cùng một danh sách cạnh được lưu, nghĩa
 * là một cạnh vô hướng chỉ được lưu một lần. Để kiểm tra có một cạnh
 * vô hướng từ \c v1 tới \c v2 hay không? - Chúng ta phải kiểm tra cả
 * hai trường hợp \c from=v1, \c to=v2 và \c from=v2, \c to=v1.
 *
 * Yêu cầu lưu trữ đồ thị với \c |V| đỉnh và \c |E| cạnh là
 * \c O(|E| + |V|)
 */

typedef struct cgraph_s {
    CGRAPH_INTEGER n;
    bool directed;
    atype(CGRAPH_INTEGER) *from;
    atype(CGRAPH_INTEGER) *to;
    atype(CGRAPH_INTEGER) *oi;
    atype(CGRAPH_INTEGER) *ii;
    atype(CGRAPH_INTEGER) *os;
    atype(CGRAPH_INTEGER) *is;
    void *attr;
} *cgraph_t;

/**
 * Kiểu trả về của cgraph_edge
 */
struct edge {
  CGRAPH_INTEGER from;
  CGRAPH_INTEGER to;
};

/**
 * Kiểu trả về của hàm tìm đường 1 đích đến
 */
struct path {
  atype(CGRAPH_INTEGER) *vertices;
  atype(CGRAPH_INTEGER) *edges;
  int reached;
};

#define make_path(name) \
  struct path *name = malloc(sizeof(struct path)); \
  name->vertices = arr_create(0, CGRAPH_INTEGER); \
  name->edges = arr_create(0, CGRAPH_INTEGER); \
  name->reached = 0

#define free_path(name) \
  arr_free(name->vertices); \
  arr_free(name->edges); \
  free(name)

/**
 * Kiểu trả về của hàm tìm đường đa đích đến.
 */
struct paths {
  atype(atype(CGRAPH_INTEGER) *) *vertices;
  atype(atype(CGRAPH_INTEGER) *) *edges;
  atype(int) *reached;
  atype(CGRAPH_INTEGER) *predecessors;
  atype(CGRAPH_INTEGER) *inbound_edges;
};
#define make_paths(name, nto, no_of_nodes) \
  struct paths *name = malloc(sizeof(struct paths)); \
  name->vertices = arr_create(nto, atype(CGRAPH_INTEGER) *); \
  name->edges = arr_create(nto, atype(CGRAPH_INTEGER) *); \
  name->reached = arr_create(nto, int); \
  for (long i = 0; i < nto; ++i) { \
    name->vertices[i] = arr_create(0, CGRAPH_INTEGER); \
    name->edges[i] = arr_create(0, CGRAPH_INTEGER); \
  } \
  name->predecessors = arr_create(no_of_nodes, CGRAPH_INTEGER); \
  name->inbound_edges = arr_create(no_of_nodes, CGRAPH_INTEGER)

#define free_paths(name) \
  for (long i = 0; i < arr_size((name)->vertices); ++i) { \
    arr_free((name)->vertices[i]); \
    arr_free((name)->edges[i]); \
  } \
  arr_free((name)->vertices); \
  arr_free((name)->edges); \
  arr_free((name)->predecessors); \
  arr_free((name)->inbound_edges); \
  arr_free((name)->reached); \
  free(name)

/**
 * Cấu trúc được trả về bởi cgraph_bfs
 */
struct bfs {
  atype(CGRAPH_INTEGER) *order;
  atype(CGRAPH_INTEGER) *rank;
  atype(CGRAPH_INTEGER) *father;
  atype(CGRAPH_INTEGER) *pred;
  atype(CGRAPH_INTEGER) *succ;
  atype(CGRAPH_INTEGER) *dist;
};

#define make_bfs(name, no_of_nodes) \
  struct bfs *name = malloc(sizeof(struct bfs)); \
  name->order = arr_create(no_of_nodes, CGRAPH_INTEGER); \
  name->rank = arr_create(no_of_nodes, CGRAPH_INTEGER); \
  name->father = arr_create(no_of_nodes, CGRAPH_INTEGER); \
  name->pred = arr_create(no_of_nodes, CGRAPH_INTEGER); \
  name->succ = arr_create(no_of_nodes, CGRAPH_INTEGER); \
  name->dist = arr_create(no_of_nodes, CGRAPH_INTEGER); \
  arr_fill(name->order, CGRAPH_NAN); \
  arr_fill(name->rank, CGRAPH_NAN); \
  arr_fill(name->father, CGRAPH_NAN); \
  arr_fill(name->pred, CGRAPH_NAN); \
  arr_fill(name->succ, CGRAPH_NAN); \
  arr_fill(name->dist, CGRAPH_NAN)

#define free_bfs(name) \
  do { \
    if (!(name)) { \
      break; \
    } \
    arr_free((name)->order); \
    arr_free((name)->rank); \
    arr_free((name)->father); \
    arr_free((name)->pred); \
    arr_free((name)->succ); \
    arr_free((name)->dist); \
    free(name); \
  } while (0)


/**
 * Cấu trúc trả về của cgraph_dfs
 */
struct dfs {
  atype(CGRAPH_INTEGER) *order;
  atype(CGRAPH_INTEGER) *order_out;
  atype(CGRAPH_INTEGER) *father;
  atype(CGRAPH_INTEGER) *dist;
};

#define make_dfs(name, no_of_nodes) \
  struct dfs *name = malloc(sizeof(struct dfs)); \
  name->order = arr_create(no_of_nodes, CGRAPH_INTEGER); \
  name->order_out = arr_create(no_of_nodes, CGRAPH_INTEGER); \
  name->father = arr_create(no_of_nodes, CGRAPH_INTEGER); \
  name->dist = arr_create(no_of_nodes, CGRAPH_INTEGER); \
  arr_fill(name->order, CGRAPH_NAN); \
  arr_fill(name->order_out, CGRAPH_NAN); \
  arr_fill(name->father, CGRAPH_NAN); \
  arr_fill(name->dist, CGRAPH_NAN)

#define free_dfs(obj) \
  do { \
    if (!obj) { \
      break; \
    } \
    arr_free(obj->order); \
    arr_free(obj->order_out); \
    arr_free(obj->father); \
    arr_free(obj->dist); \
    free(obj); \
  } while (0)


#endif  // BASE_CGRAPH_DATATYPE_H_
