#ifndef CGRAPH_IQUEUE_H_
#define CGRAPH_IQUEUE_H_

/*
  Hàng đợi cho các giá trị kiểu số nguyên,
    - được triển khai bằng cách đóng gói CGraphQueue
*/

#include "cgraph_datatype.h"
#include "cgraph_queue.h"

typedef CGraphQueue *cgraph_iqueue_t;
typedef const CGraphQueue *cgraph_iqueue_const_t;

#ifdef __cplusplus
extern "C"{
#endif

cgraph_iqueue_t cgraph_iqueue_create();

int cgraph_iqueue_peek(cgraph_iqueue_const_t const q, CGRAPH_INTEGER *out);
int cgraph_iqueue_poll(cgraph_iqueue_t q, CGRAPH_INTEGER *out);
int cgraph_iqueue_enqueue(cgraph_iqueue_t q, CGRAPH_INTEGER element);

size_t cgraph_iqueue_size(cgraph_iqueue_const_t const q);
bool cgraph_iqueue_empty(cgraph_iqueue_t const q);

void cgraph_iqueue_free(cgraph_iqueue_t *q);

#ifdef __cplusplus
}
#endif

#endif  // CGRAPH_IQUEUE_H_