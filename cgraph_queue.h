#ifndef CGRAPH_MINIMAL_GENERIC_QUEUE_H_
#define CGRAPH_MINIMAL_GENERIC_QUEUE_H_

#include <stdbool.h>

typedef struct cgraph_queue_s CGraphQueue;

#ifdef __cplusplus
extern "C"{
#endif 

CGraphQueue* cgraph_queue_create();
void cgraph_queue_free(CGraphQueue *q);
int cgraph_queue_enqueue(CGraphQueue *q, void *value);
int cgraph_queue_poll(CGraphQueue *q, void **out);
int cgraph_queue_peek(const CGraphQueue *const q, void **out);
size_t cgraph_queue_size(const CGraphQueue *const q);
bool cgraph_queue_empty(const CGraphQueue *const q);

#ifdef __cplusplus
}
#endif 

#endif // CGRAPH_MINIMAL_GENERIC_QUEUE_H_