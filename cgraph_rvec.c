#include <stdio.h>
#include <stdlib.h>

#include "cgraph_rvec.h"

cgraph_rvec_t cgraph_rvec_create() {
  return (cgraph_rvec_t)&((size_t*)calloc(2, sizeof(size_t)))[2];
}

int cgraph_rvec_setsize(cgraph_rvec_t const v, CGRAPH_INTEGER newsize) {
  CGRAPH_INTEGER capacity = (CGRAPH_INTEGER) cgraph_rvec_capacity(v);
  if (newsize <= capacity && v) {
    ((size_t *)(v))[-2] = (newsize);
    return 0;
  }
  return 1;
}

CGRAPH_INTEGER cgraph_rvec_capacity(cgraph_rvec_t const v) {
  return ((v) ? ((size_t *)(v))[-1] : (size_t)0);
}

CGRAPH_INTEGER cgraph_rvec_size(cgraph_rvec_t const v) {
  return ((v) ? ((size_t *)(v))[-2] : (size_t)0);
}

int cgraph_rvec_print(cgraph_rvec_t const v) {
  printf("size: %lld, cap: %lld, elems: ",
              (long long)cgraph_rvec_size(v),
              (long long)cgraph_rvec_capacity(v));
  for (CGRAPH_INTEGER i = 0; i < cgraph_rvec_size(v); ++i) {
    printf(" %Lf", (long double)v[i]);
  }
  printf("\n");
  return 0;
}

int cgraph_rvec_grow(cgraph_rvec_t *v, CGRAPH_INTEGER newcapacity) {
  CGRAPH_INTEGER capacity = (CGRAPH_INTEGER) cgraph_rvec_capacity((*v));
  if (capacity < newcapacity) {
    const size_t __sz = newcapacity * sizeof(CGRAPH_REAL) + (sizeof(size_t) * 2);
    size_t *__p1 = &((size_t *)(*v))[-2];
    size_t *__p2 = realloc(__p1, (__sz));
    assert(__p2);
    (*v) = (cgraph_rvec_t)(&__p2[2]);
    cgraph_rvec_t vec = *v;
    if (vec) {
      ((size_t *)(vec))[-1] = newcapacity;
    }
  }
  return 0;
}

int cgraph_rvec_init(cgraph_rvec_t *v, CGRAPH_INTEGER size) {
  cgraph_rvec_grow(v, size);
  cgraph_rvec_setsize(*v, size);
  return 0;
}

int cgraph_rvec_push_back(cgraph_rvec_t *v, CGRAPH_REAL value) {
  size_t __cap = cgraph_rvec_capacity(*v);
  if (__cap <= cgraph_rvec_size(*v)) {
    cgraph_rvec_grow(v, !__cap ? __cap + 1 : __cap * 2);
  }
  cgraph_rvec_t vec = *v;
  vec[cgraph_rvec_size(vec)] = (value);
  cgraph_rvec_setsize((vec), cgraph_rvec_size(vec) + 1);
  return 0;
}

int cgraph_rvec_free(cgraph_rvec_t *v) {
  cgraph_rvec_t vec = *v;
  if (vec) {
    size_t *p1 = &((size_t *)(vec))[-2];
    free(p1);
  }
  return 0;
}
