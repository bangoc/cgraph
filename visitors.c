#include <stdlib.h>

#include "cgraph_arr.h"
#include "cgraph_error.h"
#include "cgraph_interface.h"
#include "cgraph_visitor.h"
#include "cgen/all.h"

int cgraph_bfs(const cgraph_t graph,
               CGRAPH_INTEGER root,
               cgraph_neimode_t mode,
               bool unreachable,
               arr_ptr(CGRAPH_INTEGER) restricted,
               arr_ptr(CGRAPH_INTEGER) *order,
               arr_ptr(CGRAPH_INTEGER) *rank,
               arr_ptr(CGRAPH_INTEGER) *father,
               arr_ptr(CGRAPH_INTEGER) *pred,
               arr_ptr(CGRAPH_INTEGER) *succ,
               arr_ptr(CGRAPH_INTEGER) *dist) {
  CGRAPH_INTEGER no_of_nodes = cgraph_vcount(graph);
  CGRAPH_INTEGER actroot = root;

  CGRAPH_INTEGER act_rank = 0;
  CGRAPH_INTEGER pred_vec = -1;

  if (root < 0 || root >= no_of_nodes) {
    CGRAPH_ERROR("Đỉnh nguồn không hợp lệ đối với BFS", CGRAPH_FAILURE);
  }

  if (restricted) {
    if (!arr_irange(restricted, 0, no_of_nodes - 1)) {
      CGRAPH_ERROR("Mã đỉnh không hợp lệ trong tập restricted", CGRAPH_FAILURE);
    }
  }

  if (mode != CGRAPH_OUT && mode != CGRAPH_IN && mode != CGRAPH_ALL) {
    CGRAPH_ERROR("Lỗi tham số chế độ", CGRAPH_FAILURE);
  }

  struct gsllist *q = gsl_create_list(NULL);

  if (!cgraph_is_directed(graph)) {
     mode = CGRAPH_ALL;
  }

  bool *added = (bool*)calloc(no_of_nodes, sizeof(bool));

/* Mark the vertices that are not in the restricted set, as already
  found. Special care must be taken for vertices that are not in
  the restricted set, but are to be used as 'root' vertices. */
  if (restricted) {
    long int i, n = arr_size(restricted);
    for (i = 0; i < no_of_nodes; ++i) {
     added[i] = true;
    }
    for (i = 0; i < n; i++) {
      added[restricted[i]] = false;
    }
  }

  /* Resize result vectors, and fill them with IGRAPH_NAN */

#define VINIT(vref) \
  if (vref) {     \
    arr_resize((*vref), no_of_nodes);   \
    arr_ifill((*vref), CGRAPH_NAN); \
  }

VINIT(order);
VINIT(rank);
VINIT(father);
VINIT(pred);
VINIT(succ);
VINIT(dist);

#undef VINIT

  int rootpos = 0;
  arr_make(neis, 0, CGRAPH_INTEGER);
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
    if (father) {
      (*father)[actroot] = -1;
    }

    pred_vec = -1;

    while (!que_is_empty(q)) {
      CGRAPH_INTEGER actvect = que_peek(q).l;
      que_deq(q);
      CGRAPH_INTEGER actdist = que_peek(q).l;
      que_deq(q);
      CGRAPH_INTEGER succ_vec;

      cgraph_neighbors(graph, &neis, actvect, mode);
      long int i, n = arr_size(neis);

      if (pred) {
        (*pred)[actvect] = pred_vec;
      }
      if (rank) {
        (*rank) [actvect] = act_rank;
      }
      if (order) {
        (*order)[act_rank++] = actvect;
      }
      if (dist) {
        (*dist)[actvect] = actdist;
      }

      for (i = 0; i < n; i++) {
        CGRAPH_INTEGER nei = neis[i];
        if (!added[nei]) {
          added[nei] = true;
          que_enq(q, gtype_l(nei));
          que_enq(q, gtype_l(actdist + 1));
          if (father) {
            (*father)[nei] = actvect;
          }
        }
      }

      if (que_is_empty(q)) {
        succ_vec = -1;
      } else {
        succ_vec = que_peek(q).l;
      }

      if (succ) {
        (*succ)[actvect] = succ_vec;
      }
      pred_vec = actvect;

    } /* while q !empty */
  } /* while (1) */
  free(added);
  arr_free(neis);
  gsl_free(q);
  return 0;
}

int cgraph_simple_bfs(const cgraph_t graph,
               CGRAPH_INTEGER root,
               cgraph_neimode_t mode,
               bool unreachable,
               arr_ptr(CGRAPH_INTEGER) *father,
               arr_ptr(CGRAPH_INTEGER) *dist) {
  return cgraph_bfs(graph,
      root,
      mode,
      unreachable,
      0,
      0,
      0,
      father,
      0,
      0,
      dist);
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
 * \return Error code.
 *
 * Time complexity: O(|V|+|E|), linear in the number of vertices and
 * edges.
 */
int cgraph_dfs(const cgraph_t graph,
               CGRAPH_INTEGER root,
               cgraph_neimode_t mode,
               bool unreachable,
               arr_ptr(CGRAPH_INTEGER) *order,
               arr_ptr(CGRAPH_INTEGER) *order_out,
               arr_ptr(CGRAPH_INTEGER) *father,
               arr_ptr(CGRAPH_INTEGER) *dist) {
  CGRAPH_INTEGER no_of_nodes = cgraph_vcount(graph);
  struct gsllist *stack = gsl_create_list(NULL);
  bool *added = (bool*)calloc(no_of_nodes, sizeof(bool));
  CGRAPH_INTEGER actroot, act_rank = 0, rank_out = 0, act_dist = 0;

  if (root < 0 || root >= no_of_nodes) {
    CGRAPH_ERROR("Lỗi đỉnh nguồn cho DFS", CGRAPH_FAILURE);
  }

  if (mode != CGRAPH_OUT && mode != CGRAPH_IN && mode != CGRAPH_ALL) {
    CGRAPH_ERROR("Lỗi tham số chế độ", CGRAPH_FAILURE);
  }

  if (!cgraph_is_directed(graph)) {
    mode = CGRAPH_ALL;
  }

/* Resize result vectors and fill them with IGRAPH_NAN */

#define VINIT(vref) \
  if (vref) {            \
    arr_resize((*vref), no_of_nodes);       \
    arr_ifill((*vref), CGRAPH_NAN);  \
  }

  VINIT(order);
  VINIT(order_out);
  VINIT(father);
  VINIT(dist);

#undef VINIT

  stk_push(stack, gtype_l(root));
  added[root] = true;
  if (father) {
    (*father)[root] = -1;
  }
  if (order) {
    (*order)[act_rank++] = root;
  }
  if (dist) {
    (*dist)[root] = 0;
  }

  arr_make(nptr, no_of_nodes, CGRAPH_INTEGER);
  arr_ptr(CGRAPH_INTEGER) *neis_cache =
    calloc(no_of_nodes, sizeof(arr_ptr(CGRAPH_INTEGER)));
  arr_ptr(CGRAPH_INTEGER) neis = NULL;
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
      if (father) {
        (*father)[actroot] = -1;
      }
      if (order) {
        (*order)[act_rank++] = actroot;
      }
      if (dist) {
        (*dist)[actroot] = 0;
      }
    }

    arr_ifill(nptr, 0);
    while (!stk_is_empty(stack)) {
      CGRAPH_INTEGER actvect = stk_top(stack).l;
      if (!neis_cache[actvect]) {
        neis_cache[actvect] = arr_create(0, CGRAPH_INTEGER);
        cgraph_neighbors(graph, neis_cache + actvect, actvect, mode);
      }
      neis = neis_cache[actvect];
      CGRAPH_INTEGER n = arr_size(neis);

      /* Search for a neighbor that was not yet visited */
      bool any = 0;
      CGRAPH_INTEGER nei,
                     *ptr = nptr + actvect;
      while (!any && (*ptr) < n) {
        nei = neis[*ptr];
        any = !(added[nei]);
        ++(*ptr);
      }
      if (any) {
        /* There is such a neighbor, add it */
        stk_push(stack, gtype_l(nei));
        added[nei] = true;
        if (father) {
          (*father)[nei] = actvect;
        }
        if (order) {
          (*order)[act_rank++] = nei;
        }
        act_dist++;
        if (dist) {
          (*dist)[nei] = act_dist;
        }
      } else {
        /* There is no such neighbor, finished with the subtree */
        stk_pop(stack);
        if (order_out) {
          (*order_out)[rank_out++] = actvect;
        }
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
  return 0;
}
