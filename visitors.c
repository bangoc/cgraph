#include <stdlib.h>

#include "base/cgraph_arr.h"
#include "base/cgraph_error.h"
#include "cgraph_interface.h"
#include "cgraph_visitor.h"
#include "cgen/all.h"

struct bfs *cgraph_bfs(const cgraph_t graph,
               CGRAPH_INTEGER root,
               cgraph_neimode_t mode,
               bool unreachable,
               atype(CGRAPH_INTEGER) *restricted) {
  cgraph_err_reset();
  CGRAPH_INTEGER no_of_nodes = cgraph_vcount(graph);
  CGRAPH_INTEGER actroot = root;

  CGRAPH_INTEGER act_rank = 0;
  CGRAPH_INTEGER pred_vec = -1;

  if (root < 0 || root >= no_of_nodes) {
    CGRAPH_ERROR("Đỉnh nguồn nằm ngoài khoảng", CGRAPH_FAILURE);
    return NULL;
  }

  if (restricted) {
    if (!arr_irange(restricted, 0, no_of_nodes - 1)) {
      CGRAPH_ERROR("Có đỉnh không hợp lệ trong tập restricted", CGRAPH_FAILURE);
      return NULL;
    }
  }

  if (mode != CGRAPH_OUT && mode != CGRAPH_IN && mode != CGRAPH_ALL) {
    CGRAPH_ERROR("Tham số mode không hợp lệ", CGRAPH_FAILURE);
    return NULL;
  }

  struct gsllist *q = gsl_create_list(NULL);

  if (!cgraph_is_directed(graph)) {
     mode = CGRAPH_ALL;
  }

  bool *added = (bool*)calloc(no_of_nodes, sizeof(bool));

/* Đánh dấu các đỉnh không có trong tập restricted như đã được tìm thấy.
   Cần xét riêng trường hợp nút không có trong restricted nhưng được
   sử dụng như gốc (root). */
  if (restricted) {
    long int i, n = arr_size(restricted);
    for (i = 0; i < no_of_nodes; ++i) {
     added[i] = true;
    }
    for (i = 0; i < n; i++) {
      added[restricted[i]] = false;
    }
  }

  make_bfs(res, no_of_nodes);

  int rootpos = 0;
  while (1) {
    if (rootpos == 0) {
      actroot = root;
      ++rootpos;
    } else if (unreachable) {
      if (rootpos == 1) {
        actroot = 0;
        ++rootpos;
      } else if (actroot + 1 < no_of_nodes) {
        ++actroot;
      } else {
        break;
      }
    } else {
      break;
    }
    if (added[actroot]) {
      continue;
    }

    que_enq(q, gtype_l(actroot));
    que_enq(q, gtype_l(0));
    added[actroot] = true;
    res->father[actroot] = -1;
    pred_vec = -1;

    while (!que_is_empty(q)) {
      CGRAPH_INTEGER actvect = que_peek(q).l;
      que_deq(q);
      CGRAPH_INTEGER actdist = que_peek(q).l;
      que_deq(q);
      CGRAPH_INTEGER succ_vec;

      atype(CGRAPH_INTEGER) *neis = cgraph_neighbors(graph, actvect, mode);
      long int i, n = arr_size(neis);

      res->pred[actvect] = pred_vec;
      res->rank[actvect] = act_rank;
      res->order[act_rank++] = actvect;
      res->dist[actvect] = actdist;

      for (i = 0; i < n; i++) {
        CGRAPH_INTEGER nei = neis[i];
        if (!added[nei]) {
          added[nei] = true;
          que_enq(q, gtype_l(nei));
          que_enq(q, gtype_l(actdist + 1));
          res->father[nei] = actvect;
        }
      }

      if (que_is_empty(q)) {
        succ_vec = -1;
      } else {
        succ_vec = que_peek(q).l;
      }

      res->succ[actvect] = succ_vec;
      pred_vec = actvect;
      arr_free(neis);
    } /* while q !empty */
  } /* while (1) */
  free(added);
  gsl_free(q);
  return res;
}

//igraph_dfs
/**
 * Depth-first search
 *
 * A simple depth-first search,
 * It is allowed to supply null pointers as the output arguments the
 * user is not interested in, in this case they will be ignored.
 *
 * </para><para>
 * If not all vertices can be reached from the supplied root vertex,
 * then additional root vertices will be used, in the order of their
 * vertex ids.
 * \param graph The input graph.
 * \param root The id of the root vertex.
 * \param mode For directed graphs, it defines which edges to follow.
 *        \c IGRAPH_OUT means following the direction of the edges,
 *        \c IGRAPH_IN means the opposite, and
 *        \c IGRAPH_ALL ignores the direction of the edges.
 *        This parameter is ignored for undirected graphs.
 * \param unreachable Logical scalar, whether the search should visit
 *        the vertices that are unreachable from the given root
 *        node(s). If true, then additional searches are performed
 *        until all vertices are visited.
 * \param order If not null pointer, then the vertex ids of the graph are
 *        stored here, in the same order as they were discovered.
 * \param order_out If not a null pointer, then the vertex ids of the
 *        graphs are stored here, in the order of the completion of
 *        their subtree.
 * \param father If not a null pointer, then the id of the father of
 *        each vertex is stored here.
 * \param dist If not a null pointer, then the distance from the root of
 *        the current search tree is stored here.
 * \param in_callback If not null, then it should be a pointer to a
 *        function of type \ref igraph_dfshandler_t. This function
 *        will be called, whenever a new vertex is discovered.
 * \param out_callback If not null, then it should be a pointer to a
 *        function of type \ref igraph_dfshandler_t. This function
 *        will be called, whenever the subtree of a vertex is completed.
 * \param extra Extra argument to pass to the callback function(s).
 * \return Trả về con trỏ tới cấu trúc dfs chứa các kết quả tìm đường.
 *
 * Time complexity: O(|V|+|E|), linear in the number of vertices and
 * edges.
 */
struct dfs *cgraph_dfs(const cgraph_t graph,
               CGRAPH_INTEGER root,
               cgraph_neimode_t mode,
               bool unreachable) {
  cgraph_err_reset();
  CGRAPH_INTEGER no_of_nodes = cgraph_vcount(graph);
  struct gsllist *stack = gsl_create_list(NULL);
  bool *added = (bool*)calloc(no_of_nodes, sizeof(bool));
  CGRAPH_INTEGER actroot, act_rank = 0,
                 rank_out = 0, act_dist = 0;

  if (root < 0 || root >= no_of_nodes) {
    CGRAPH_ERROR("Đỉnh nguồn nằm ngoài khoảng", CGRAPH_FAILURE);
    return NULL;
  }

  if (mode != CGRAPH_OUT && mode != CGRAPH_IN &&
      mode != CGRAPH_ALL) {
    CGRAPH_ERROR("Tham số mode không hợp lệ", CGRAPH_FAILURE);
    return NULL;
  }

  if (!cgraph_is_directed(graph)) {
    mode = CGRAPH_ALL;
  }

  stk_push(stack, gtype_l(root));
  added[root] = true;
  make_dfs(res, no_of_nodes);
  res->father[root] = -1;
  res->order[act_rank++] = root;
  res->dist[root] = 0;

  arr_make(nptr, no_of_nodes, CGRAPH_INTEGER);
  atype(CGRAPH_INTEGER) **neis_cache =
    calloc(no_of_nodes, sizeof(atype(CGRAPH_INTEGER) *));
  atype(CGRAPH_INTEGER) *neis = NULL;
  for (actroot = 0; actroot < no_of_nodes; ) {

    /* Đầu tiên là gốc và sau đó là các đỉnh khác */
    if (stk_is_empty(stack)) {
      if (!unreachable) {
        break;
      }
      if (added[actroot]) {
        actroot++;
        continue;
      }
      stk_push(stack, gtype_l(actroot));
      added[actroot] = true;
      res->father[actroot] = -1;
      res->order[act_rank++] = actroot;
      res->dist[actroot] = 0;
    }

    arr_ifill(nptr, 0);
    while (!stk_is_empty(stack)) {
      CGRAPH_INTEGER actvect = stk_top(stack).l;
      if (!neis_cache[actvect]) {
        neis_cache[actvect] = cgraph_neighbors(graph, actvect, mode);
      }
      neis = neis_cache[actvect];
      CGRAPH_INTEGER n = arr_size(neis);

      /* Search for a neighbor that was not yet visited */
      bool any = 0;
      CGRAPH_INTEGER nei, *ptr = nptr + actvect;
      while (!any && (*ptr) < n) {
        nei = neis[*ptr];
        any = !(added[nei]);
        ++(*ptr);
      }
      if (any) {
        /* There is such a neighbor, add it */
        stk_push(stack, gtype_l(nei));
        added[nei] = true;
        res->father[nei] = actvect;
        res->order[act_rank++] = nei;
        act_dist++;
        res->dist[nei] = act_dist;
      } else {
        /* There is no such neighbor, finished with the subtree */
        stk_pop(stack);
        res->order_out[rank_out++] = actvect;
        act_dist--;
      }
    }
  }
  for (CGRAPH_INTEGER i = 0; i < no_of_nodes; ++i) {
    if (neis_cache[i]) {
      arr_free(neis_cache[i]);
    }
  }
  free(neis_cache);
  arr_free(nptr);
  gsl_free(stack);
  free(added);
  return res;
}
