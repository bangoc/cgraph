#include <stdlib.h>

#include "cgraph_iqueue.h"

cgraph_iqueue_t cgraph_iqueue_create() {
  return cgraph_queue_create();
}

int cgraph_iqueue_peek (cgraph_iqueue_const_t const q, CGRAPH_INTEGER *out) {
  void *data;
  int ret = cgraph_queue_peek(q, &data);
  if (ret != 0) {
    return ret;
  }
  *out = *((CGRAPH_INTEGER*)data);
  return 0;
}

int cgraph_iqueue_poll (cgraph_iqueue_t q, CGRAPH_INTEGER *out) {
  void *data;
  int ret = cgraph_queue_poll(q, &data);
  if (ret != 0) {
    return ret;
  }
  *out = *((CGRAPH_INTEGER*)data);
  free(data);
  return 0;
}

int cgraph_iqueue_enqueue(cgraph_iqueue_t q, CGRAPH_INTEGER element) {
  CGRAPH_INTEGER *data = (CGRAPH_INTEGER*)malloc(sizeof(CGRAPH_INTEGER));
  *data = element;
  return cgraph_queue_enqueue(q, data);
}

size_t cgraph_iqueue_size(cgraph_iqueue_const_t const q) {
  return cgraph_queue_size(q);
}

void cgraph_iqueue_free(cgraph_iqueue_t *q) {
  cgraph_queue_free(*q);
}

bool cgraph_iqueue_empty(cgraph_iqueue_t const q) {
  return cgraph_queue_empty(q);
}