#include <stdlib.h>

#include "cgraph_istack.h"
#include "cgraph_ivec.h"

struct cgraph_istack_s {
  cgraph_ivec_t items;
};

cgraph_istack_t cgraph_istack_create() {
  cgraph_istack_t s = (cgraph_istack_t)malloc(sizeof(CGraphIStack));
  if (s != NULL) {
    s->items = cgraph_ivec_create();
  }
  return s;
}

int cgraph_istack_push(cgraph_istack_t s, CGRAPH_INTEGER element) {
  return cgraph_ivec_push_back(&(s->items), element);
}

int cgraph_istack_pop(cgraph_istack_t s, CGRAPH_INTEGER *out) {
  int ret = 0;
  if (out) {
    ret = cgraph_istack_top(s, out);
  }
  if (ret != 0) {
    return ret;
  }
  return cgraph_ivec_setsize(s->items, cgraph_ivec_size(s->items) - 1);
}

void cgraph_istack_free(cgraph_istack_t *s) {
  cgraph_ivec_free(&((*s)->items));
  free(*s);
}

int cgraph_istack_top(cgraph_istack_const_t const s, CGRAPH_INTEGER *out) {
  size_t sz = cgraph_ivec_size(s->items);
  if (sz == 0) {
    return 1;
  }
  *out = (s->items)[sz - 1];
  return 0;
}

size_t cgraph_istack_size(cgraph_istack_const_t const s) {
  return cgraph_ivec_size(s->items);
}

bool cgraph_istack_empty(cgraph_istack_const_t const s) {
  return cgraph_istack_size(s) == 0;
}
