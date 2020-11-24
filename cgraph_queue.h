#ifndef CGRAPH_QUEUE_H_
#define CGRAPH_QUEUE_H_

/*
  Collections-C queue wrapper for integer type
*/

#include "cgraph_datatype.h"
#include "ctc/queue.h"

typedef Queue *cgraph_iqueue_t;
typedef const Queue *cgraph_iqueue_const_t;

cgraph_iqueue_t cgraph_queue_create();

int cgraph_iqueue_peek(cgraph_iqueue_const_t const q, CGRAPH_INTEGER *out);
int cgraph_iqueue_poll(cgraph_iqueue_t q, CGRAPH_INTEGER *out);
int cgraph_iqueue_enqueue(cgraph_iqueue_t q, CGRAPH_INTEGER element);

size_t cgraph_iqueue_size(cgraph_iqueue_const_t const q);

void cgraph_iqueue_destroy(cgraph_iqueue_t q);

#endif  // CGRAPH_QUEUE_H_