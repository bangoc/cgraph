#include "cgraph_queue.h"

cgraph_iqueue_t cgraph_queue_create() {
  cgraph_iqueue_t q;
  if (queue_new(&q) != CC_ERR_ALLOC) {
    return q;
  }
  return NULL;
}