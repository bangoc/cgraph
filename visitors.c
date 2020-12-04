#include <stdlib.h>

#include "cgraph_error.h"
#include "cgraph_interface.h"
#include "cgraph_iqueue.h"
#include "cgraph_istack.h"
#include "cgraph_ivec.h"
#include "cgraph_visitor.h"

// igraph_bfs
int cgraph_bfs(const cgraph_t *graph,
               CGRAPH_INTEGER root,
               cgraph_neimode_t mode,
               bool unreachable,
               cgraph_ivec_t const restricted,
               cgraph_ivec_t *order,
               cgraph_ivec_t *rank,
               cgraph_ivec_t *father,
               cgraph_ivec_t *pred,
               cgraph_ivec_t *succ,
               cgraph_ivec_t *dist) {
  cgraph_iqueue_t q = cgraph_iqueue_create();
  CGRAPH_INTEGER no_of_nodes = cgraph_vcount(graph);
  CGRAPH_INTEGER actroot = root;

  CGRAPH_INTEGER act_rank = 0;
  CGRAPH_INTEGER pred_vec = -1;

  if (root < 0 || root >= no_of_nodes) {
    CGRAPH_ERROR("Invalid root vertex in BFS");
  }

  if (restricted) {
    CGRAPH_INTEGER min, max;
    cgraph_ivec_minmax(restricted, &min, &max);
    if (min < 0 || max >= no_of_nodes) {
      CGRAPH_ERROR("Invalid vertex id in restricted set");
    }
  }

  if (mode != CGRAPH_OUT && mode != CGRAPH_IN && mode != CGRAPH_ALL) {
    CGRAPH_ERROR("Invalid mode argument");
  }

  if (!cgraph_is_directed(graph)) {
     mode = CGRAPH_ALL;
  }

  bool *added = (bool*)calloc(no_of_nodes, sizeof(bool));

/* Mark the vertices that are not in the restricted set, as already
  found. Special care must be taken for vertices that are not in
  the restricted set, but are to be used as 'root' vertices. */
  if (restricted) {
    long int i, n = cgraph_ivec_size(restricted);
    for (i = 0; i < no_of_nodes; ++i) {
     added[i] = true;
    }
    for (i = 0; i < n; i++) {
      added[restricted[i]] = false;
    }
  }

  /* Resize result vectors, and fill them with IGRAPH_NAN */

# define VINIT(v) if (v) {                      \
   cgraph_ivec_init((v), no_of_nodes);   \
   cgraph_ivec_fill((*v), CGRAPH_NAN); }

VINIT(order);
VINIT(rank);
VINIT(father);
VINIT(pred);
VINIT(succ);
VINIT(dist);
# undef VINIT

  int rootpos = 0;
  cgraph_ivec_t neis = cgraph_ivec_create();
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

    CGRAPH_CHECK(cgraph_iqueue_enqueue(q, actroot));
    CGRAPH_CHECK(cgraph_iqueue_enqueue(q, 0));
    added[actroot] = true;
    if (father) {
      (*father)[actroot] = -1;
    }

    pred_vec = -1;

    while (!cgraph_iqueue_empty(q)) {
      CGRAPH_INTEGER actvect;
      cgraph_iqueue_poll(q, &actvect);
      CGRAPH_INTEGER actdist;
      cgraph_iqueue_poll(q, &actdist);
      CGRAPH_INTEGER succ_vec;

      cgraph_neighbors(graph, &neis, actvect, mode);
      long int i, n = cgraph_ivec_size(neis);

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
          CGRAPH_CHECK(cgraph_iqueue_enqueue(q, nei));
          CGRAPH_CHECK(cgraph_iqueue_enqueue(q, actdist + 1));
          if (father) {
            (*father)[nei] = actvect;
          }
        }
      }

      if (cgraph_iqueue_empty(q)) {
        succ_vec = -1;
      } else {
        cgraph_iqueue_peek(q, &succ_vec);
      }

      if (succ) {
        (*succ)[actvect] = succ_vec;
      }
      pred_vec = actvect;

    } /* while q !empty */
  } /* while (1) */
  free(added);
  cgraph_ivec_free(&neis);
  cgraph_iqueue_free(&q);
  return 0;
}

int cgraph_simple_bfs(const cgraph_t *graph,
               CGRAPH_INTEGER root,
               cgraph_neimode_t mode,
               bool unreachable,
               cgraph_ivec_t *father,
               cgraph_ivec_t *dist) {
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
int cgraph_dfs(const cgraph_t *graph,
               CGRAPH_INTEGER root,
               cgraph_neimode_t mode,
               bool unreachable,
               cgraph_ivec_t *order,
               cgraph_ivec_t *order_out,
               cgraph_ivec_t *father,
               cgraph_ivec_t *dist) {
  CGRAPH_INTEGER no_of_nodes = cgraph_vcount(graph);
  cgraph_istack_t stack = cgraph_istack_create();
  bool *added = (bool*)calloc(no_of_nodes, sizeof(bool));
  CGRAPH_INTEGER actroot, act_rank = 0, rank_out = 0, act_dist = 0;

  if (root < 0 || root >= no_of_nodes) {
    CGRAPH_ERROR("Invalid root vertex for DFS");
  }

  if (mode != CGRAPH_OUT && mode != CGRAPH_IN && mode != CGRAPH_ALL) {
    CGRAPH_ERROR("Invalid mode argument");
  }

  if (!cgraph_is_directed(graph)) {
    mode = CGRAPH_ALL;
  }

/* Resize result vectors and fill them with IGRAPH_NAN */

# define VINIT(v) if (v) {                \
  cgraph_ivec_init(v, no_of_nodes);       \
  cgraph_ivec_fill((*v), CGRAPH_NAN); }

  VINIT(order);
  VINIT(order_out);
  VINIT(father);
  VINIT(dist);

# undef VINIT

  CGRAPH_CHECK(cgraph_istack_push(stack, root));
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

  cgraph_ivec_t nptr = cgraph_ivec_create();
  cgraph_ivec_init(&nptr, no_of_nodes);
  cgraph_ivec_t *neis_cache = calloc(no_of_nodes, sizeof(cgraph_ivec_t));
  cgraph_ivec_t neis = NULL;
  for (actroot = 0; actroot < no_of_nodes; ) {

    /* 'root' first, then all other vertices */
    if (cgraph_istack_empty(stack)) {
      if (!unreachable) {
        break;
      }
      if (added[actroot]) {
        actroot++;
        continue;
      }
      CGRAPH_CHECK(cgraph_istack_push(stack, actroot));
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

    cgraph_ivec_fill(nptr, 0);
    while (!cgraph_istack_empty(stack)) {
      CGRAPH_INTEGER actvect;
      cgraph_istack_top(stack, &actvect);
      if (!neis_cache[actvect]) {
        neis_cache[actvect] = cgraph_ivec_create();
        cgraph_neighbors(graph, neis_cache + actvect, actvect, mode);
      }
      neis = neis_cache[actvect];
      CGRAPH_INTEGER n = cgraph_ivec_size(neis);

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
        CGRAPH_CHECK(cgraph_istack_push(stack, nei));
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
        cgraph_istack_pop(stack, NULL);
        if (order_out) {
          (*order_out)[rank_out++] = actvect;
        }
        act_dist--;
      }
    }
  }
  for (CGRAPH_INTEGER i = 0; i < no_of_nodes; ++i) {
    if (neis_cache[i]) {
      cgraph_ivec_free(neis_cache + i);
    }
  }
  free(neis_cache);
  cgraph_ivec_free(&nptr);
  cgraph_istack_free(&stack);
  free(added);
  return 0;
}
