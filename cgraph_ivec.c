#include <stdio.h>
#include <stdlib.h>

#include "cgraph_ivec.h"
#include "cgraph_error.h"

cgraph_ivec_t cgraph_ivec_create() {
  return (cgraph_ivec_t)&((size_t*)calloc(2, sizeof(size_t)))[2];
}

cgraph_ivec_t cgraph_ivec_from_array(CGRAPH_INTEGER *a,
                                    CGRAPH_INTEGER n) {
  cgraph_ivec_t v = cgraph_ivec_create();
  int ret = 0;
  for (CGRAPH_INTEGER i = 0; i < n; ++i) {
    ret += cgraph_ivec_push_back(&v, a[i]);
  }
  if (ret) {
    CGRAPH_ERROR("Can not create a vector from an array");
    cgraph_ivec_free(&v);
  }
  return ret ==0? v: NULL;
}

CGRAPH_INTEGER cgraph_ivec_max(cgraph_ivec_t const v) {
  CGRAPH_INTEGER max = v[0];
  for (CGRAPH_INTEGER i = 1; i < cgraph_ivec_size(v); ++i) {
    if (v[i] > max) {
      max = v[i];
    }
  }
  return max;
}

int cgraph_ivec_minmax(cgraph_ivec_t const v, CGRAPH_INTEGER *min, CGRAPH_INTEGER *max) {
  CGRAPH_INTEGER _min, _max;
  _min = _max = v[0];
  for (CGRAPH_INTEGER i = 1; i < cgraph_ivec_size(v); ++i) {
    if (v[i] > _max) {
      _max = v[i];
    } else if (v[i] < _min) {
      _min = v[i];
    }
  }
  if (min) {
    *min = _min;
  }
  if (max) {
    *max = _max;
  }
  if (!min && !max) {
    return 1;
  }
  return 0;
}

bool cgraph_ivec_isininterval(cgraph_ivec_t const v,
                              CGRAPH_INTEGER low,
                              CGRAPH_INTEGER high) {
  for (CGRAPH_INTEGER i = 0; i < cgraph_ivec_size(v); ++i) {
    if (v[i] < low || v[i] > high) {
      return false;
    }
  }
  return true;
}

static cgraph_ivec_t _v;
static cgraph_ivec_t _v2;
int ref_cmp(const void *o1, const void *o2) {
  CGRAPH_INTEGER i1 = *((CGRAPH_INTEGER*)o1),
                i2 = *((CGRAPH_INTEGER*)o2);
  if (_v[i1] < _v[i2]) {
    return -1;
  } else if (_v[i1] > _v[i2]) {
    return 1;
  }
  return _v2[i1] - _v2[i2];
}

int cgraph_ivec_order(cgraph_ivec_t const v,
                      cgraph_ivec_t const v2,
                      cgraph_ivec_t const res) {
  _v = v;
  _v2 = v2;
  CGRAPH_INTEGER n = cgraph_ivec_size(v);
  for (int i = 0; i < n; ++i) {
    res[i] = i;
  }
  qsort(res, n, sizeof(CGRAPH_INTEGER), ref_cmp);
  return 0;
}

int cgraph_ivec_null(cgraph_ivec_t const v) {
  cgraph_ivec_fill(v, 0);
  return 0;
}

int cgraph_ivec_setsize(cgraph_ivec_t const v, CGRAPH_INTEGER newsize) {
  CGRAPH_INTEGER capacity = (CGRAPH_INTEGER) cgraph_ivec_capacity(v);
  if (newsize <= capacity && v) {
    ((size_t *)(v))[-2] = (newsize);
    return 0;
  }
  return 1;
}

CGRAPH_INTEGER cgraph_ivec_capacity(cgraph_ivec_t const v) {
  return ((v) ? ((size_t *)(v))[-1] : (size_t)0);
}

CGRAPH_INTEGER cgraph_ivec_size(cgraph_ivec_t const v) {
  return ((v) ? ((size_t *)(v))[-2] : (size_t)0);
}

int cgraph_ivec_fill(cgraph_ivec_t const v, CGRAPH_INTEGER data) {
  size_t sz = cgraph_ivec_size(v);
  for (CGRAPH_INTEGER i = 0; i < sz; ++i) {
    v[i] = data;
  }
  return 0;
}

int cgraph_ivec_print(cgraph_ivec_t const v) {
  printf("size: %lld, cap: %lld, elems: ",
              (long long)cgraph_ivec_size(v),
              (long long)cgraph_ivec_capacity(v));
  for (CGRAPH_INTEGER i = 0; i < cgraph_ivec_size(v); ++i) {
    printf(" %lld", (long long)v[i]);
  }
  printf("\n");
  return 0;
}

int cgraph_ivec_grow(cgraph_ivec_t *v, CGRAPH_INTEGER newcapacity) {
  CGRAPH_INTEGER capacity = (CGRAPH_INTEGER) cgraph_ivec_capacity((*v));
  if (capacity < newcapacity) {
    const size_t __sz = newcapacity * sizeof(CGRAPH_INTEGER) + (sizeof(size_t) * 2);
    size_t *__p1 = &((size_t *)(*v))[-2];
    size_t *__p2 = (size_t *)realloc(__p1, (__sz));
    assert(__p2);
    (*v) = (cgraph_ivec_t)(&__p2[2]);
    cgraph_ivec_t vec = *v;
    if (vec) {
      ((size_t *)(vec))[-1] = newcapacity;
    }
  }
  return 0;
}

int cgraph_ivec_init(cgraph_ivec_t *v, CGRAPH_INTEGER size) {
  cgraph_ivec_grow(v, size);
  cgraph_ivec_setsize(*v, size);
  return 0;
}

int cgraph_ivec_push_back(cgraph_ivec_t *v, CGRAPH_INTEGER value) {
  size_t __cap = cgraph_ivec_capacity(*v);
  if (__cap <= cgraph_ivec_size(*v)) {
    cgraph_ivec_grow(v, !__cap ? __cap + 1 : __cap * 2);
  }
  cgraph_ivec_t vec = *v;
  vec[cgraph_ivec_size(vec)] = (value);
  cgraph_ivec_setsize((vec), cgraph_ivec_size(vec) + 1);
  return 0;
}

int cgraph_ivec_append(cgraph_ivec_t *v,
                CGRAPH_INTEGER *a, CGRAPH_INTEGER n) {
  int ret = 0;
  for (CGRAPH_INTEGER i = 0; i < n; ++i) {
    ret += cgraph_ivec_push_back(v, a[i]);
  }
  return ret;
}

int cgraph_ivec_free(cgraph_ivec_t *v) {
  cgraph_ivec_t vec = *v;
  if (vec) {
    size_t *p1 = &((size_t *)(vec))[-2];
    free(p1);
  }
  return 0;
}
