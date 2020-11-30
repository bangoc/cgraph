#include <stdlib.h>

#include "cgraph_vector.h"

CGRAPH_INTEGER cgraph_ivec_max(cgraph_ivec_t const v) {
  CGRAPH_INTEGER max = v[0];
  for (CGRAPH_INTEGER i = 1; i < cvector_size(v); ++i) {
    if (v[i] > max) {
      max = v[i];
    }
  }
  return max;
}

int cgraph_ivec_minmax(cgraph_ivec_t const v, CGRAPH_INTEGER *min, CGRAPH_INTEGER *max) {
  CGRAPH_INTEGER _min, _max; 
  _min = _max = v[0];
  for (CGRAPH_INTEGER i = 1; i < cvector_size(v); ++i) {
    if (v[i] > _max) {
      _max = v[i];
    }
    if (v[i] < _min) {
      _min = v[i];
    }
  }
  *min = _min;
  *max = _max;
  return 0;
}

bool cgraph_ivec_isininterval(cgraph_ivec_t const v, 
                              CGRAPH_INTEGER low, 
                              CGRAPH_INTEGER high) {
  for (CGRAPH_INTEGER i = 0; i < cvector_size(v); ++i) {
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
  CGRAPH_INTEGER n = cvector_size(v);
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
  CGRAPH_INTEGER capacity = (CGRAPH_INTEGER) cvector_capacity(v);
  if (newsize <= capacity) {
    cvector_set_size(v, newsize);
    return 0;
  }
  return 1;
}

CGRAPH_INTEGER cgraph_ivec_capacity(cgraph_ivec_t const v) {
  return (CGRAPH_INTEGER)cvector_capacity(v);
}

CGRAPH_INTEGER cgraph_ivec_size(cgraph_ivec_t const v) {
  return (CGRAPH_INTEGER)cvector_size(v);
}

int cgraph_ivec_fill(cgraph_ivec_t const v, CGRAPH_INTEGER data) {
  size_t sz = cvector_size(v);
  for (CGRAPH_INTEGER i = 0; i < sz; ++i) {
    v[i] = data;
  }
  return 0;
}

cgraph_ivec_t cgraph_ivec_create() {
  return (cgraph_ivec_t)cvector_create_empty();
}


int cgraph_ivec_grow(cgraph_ivec_t *v, CGRAPH_INTEGER newcapacity) {
  CGRAPH_INTEGER capacity = (CGRAPH_INTEGER) cvector_capacity((*v));
  if (capacity < newcapacity) { 
    cvector_grow((*v), newcapacity);
  }
  return 0;
}

int cgraph_ivec_init(cgraph_ivec_t *v, CGRAPH_INTEGER size) {
  cgraph_ivec_grow(v, size);
  cgraph_ivec_setsize(*v, size);
  return 0;
}

int cgraph_ivec_push_back(cgraph_ivec_t *v, CGRAPH_INTEGER value) {
  cvector_push_back((*v), ((CGRAPH_INTEGER)value));
  return 0;
}

int cgraph_ivec_free(cgraph_ivec_t *v) {
  cvector_free((*v));
  return 0;
}
