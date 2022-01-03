#ifndef CGEN_EXT_H_
#define CGEN_EXT_H_

/*
  (C) Nguyen Ba Ngoc 2022
*/

#include "cgraph_ivec.h"
#include "cgen/base/gtype.h"

#define ivec_at(vv, i) (*(gtype_ivec_ref(gvec_elem(vv, i))))

static inline void gtype_free_ivec_ref(gtype value) {
  cgraph_ivec_t *tmp = (cgraph_ivec_t*)(value.v);
  cgraph_ivec_free(tmp);
  free(tmp);
}

static inline cgraph_ivec_t *gtype_ivec_ref(gtype value) {
  cgraph_ivec_t *tmp = (cgraph_ivec_t*)(value.v);
  return tmp;
}

#endif  // CGEN_EXT_H_