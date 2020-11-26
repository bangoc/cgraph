#include "cgraph_error.h"
#include "cgraph_interface.h"
#include "cgraph_queue.h"
#include "cgraph_vector.h"
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

  CGRAPH_CHECK(cgraph_iqueue_enqueue(q, actroot));
  CGRAPH_CHECK(cgraph_iqueue_enqueue(q, 0));
  added[actroot] = true;
  if (father) {
    (*father)[actroot] = -1;
  }

  pred_vec = -1;

  cgraph_ivec_t neis = cgraph_ivec_create();
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
      if (! added[nei]) {
        added[nei] = 1;
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
  free(added);
  cgraph_ivec_free(&neis);
  cgraph_iqueue_free(&q);
  return 0;
}