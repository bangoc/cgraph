#include "cgraph_queue.h"

cgraph_iqueue_t cgraph_queue_create() {
  cgraph_iqueue_t q;
  if (queue_new(&q) != CC_ERR_ALLOC) {
    return q;
  }
  return NULL;
}

int cgraph_iqueue_peek (cgraph_iqueue_const_t const q, CGRAPH_INTEGER *out) {
  void *data;
  if (queue_peek(q, &data) == CC_ERR_OUT_OF_RANGE) {
    return 1;
  }
  *out = *((CGRAPH_INTEGER*)data);
  return 0;
}

int cgraph_iqueue_poll (cgraph_iqueue_t q, CGRAPH_INTEGER *out) {
  void *data;
  if (queue_poll(q, &data) == CC_ERR_OUT_OF_RANGE) {
    return 1;
  }
  *out = *((CGRAPH_INTEGER*)data);
  return 0;
}

int cgraph_iqueue_enqueue(cgraph_iqueue_t q, CGRAPH_INTEGER element) {
  CGRAPH_INTEGER *data = (CGRAPH_INTEGER*)malloc(sizeof(CGRAPH_INTEGER));
  *data = element;
  if (queue_enqueue(q, data) == CC_ERR_ALLOC) {
    free(data);
    return 1;
  }
  return 0;
}

size_t cgraph_iqueue_size(cgraph_iqueue_const_t const q) {
  return queue_size(q);
}

void cgraph_iqueue_destroy(cgraph_iqueue_t q) {
  queue_destroy(q);
}