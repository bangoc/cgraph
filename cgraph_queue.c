#include <stdlib.h>

#include "cgraph_queue.h"

typedef struct _cgraph_queue_node {
  void *value;
  struct _cgraph_queue_node *next;
} CGraphQueueNode;

struct cgraph_queue_s {
  int size;
  CGraphQueueNode *head;
  CGraphQueueNode *tail;
};

CGraphQueue* cgraph_queue_create() {
  CGraphQueue *q;
  q = malloc(sizeof(CGraphQueue));

  if (q == NULL) {
    return q;
  }

  q->size = 0;
  q->head = NULL;
  q->tail = NULL;

  return q;
}

void cgraph_queue_free(CGraphQueue *q) {
  while (q->head != NULL) {
    CGraphQueueNode *tmp = q->head;
    q->head = q->head->next;
    if (tmp->value != NULL) {
      free(tmp->value);
    }
    free(tmp);
  }

  free (q);
}

int cgraph_queue_enqueue(CGraphQueue *q, void *value) {
  CGraphQueueNode *node = malloc(sizeof(CGraphQueueNode));

  if (node == NULL) {
    return 1;
  }

  node->value = value;
  node->next = NULL;

  if (q->head == NULL) {
    q->head = node;
    q->tail = node;
    q->size = 1;
    return 0;
  }

  q->tail->next = node;
  q->tail = node;
  q->size += 1;

  return 0;
}

int cgraph_queue_poll(CGraphQueue *q, void **out) {
  int ret = cgraph_queue_peek(q, out);
  if (ret != 0) {
    return ret;
  }

  CGraphQueueNode *tmp = q->head;
  q->head = q->head->next;
  q->size -= 1;
  free(tmp);
  /*
  client take the responsibility to free value
  */

  return 0;
}

int cgraph_queue_peek(const CGraphQueue *const q, void **out) {
  if (q->size == 0) {
    *out = NULL;
    return 1;
  }
  *out = q->head->value;
  return 0;
}

size_t cgraph_queue_size(const CGraphQueue * const q) {
  return q->size;
}

bool cgraph_queue_empty(const CGraphQueue *const q) {
  return q->size == 0;
}