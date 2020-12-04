#ifndef CGRAPH_ISTACK_H_
#define CGRAPH_ISTACK_H_

#include "cgraph_datatype.h"

#ifdef __cplusplus
extern "C"{
#endif 

typedef struct cgraph_istack_s CGraphIStack;
typedef CGraphIStack *cgraph_istack_t;
typedef const CGraphIStack *cgraph_istack_const_t;

cgraph_istack_t cgraph_istack_create();
int cgraph_istack_pop(cgraph_istack_t s, CGRAPH_INTEGER *out);
int cgraph_istack_push(cgraph_istack_t s, CGRAPH_INTEGER element);
void cgraph_istack_free(cgraph_istack_t *s);

int cgraph_istack_top(cgraph_istack_const_t const s, CGRAPH_INTEGER *out);
size_t cgraph_istack_size(cgraph_istack_const_t const s);
bool cgraph_istack_empty(cgraph_istack_const_t const s);

#ifdef __cplusplus
}
#endif 

#endif  // CGRAPH_ISTACK_H_