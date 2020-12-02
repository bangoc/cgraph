#include "cgraph_stack.h"

cgraph_istack_t cgraph_istack_create() {
  cgraph_istack_t s;
  if (stack_new(&s) == CC_OK) {
    return s;
  }
  return NULL;
}

int cgraph_istack_push(cgraph_istack_t s, CGRAPH_INTEGER element) {
  CGRAPH_INTEGER *data = (CGRAPH_INTEGER*)malloc(sizeof(CGRAPH_INTEGER));
  *data = element;
  if (stack_push(s, data) == CC_OK) {
    return 0;
  }
  return 1;
}

int cgraph_istack_pop(cgraph_istack_t s, CGRAPH_INTEGER *out) {
  void *data;
  if (stack_pop(s, &data) == CC_OK) {
    if (out) {
      *out = *((CGRAPH_INTEGER*)data);
    }
    return 0;
  }
  return 1;
}

void cgraph_istack_free(cgraph_istack_t *s) {
  stack_destroy(*s);
}

int cgraph_istack_top(cgraph_istack_const_t const s, CGRAPH_INTEGER *out) {
  void *data;
  if (stack_peek(s, &data) == CC_OK) {
    *out = *((CGRAPH_INTEGER*)data);
    return 0;
  }
  return 1;
}

size_t cgraph_istack_size(cgraph_istack_const_t const s) {
  return stack_size(s);
}

bool cgraph_istack_empty(cgraph_istack_const_t const s) {
  return cgraph_istack_size(s) == 0;
}
