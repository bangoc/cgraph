#include "cgen.h"
#include "cgen_ext.h"

int main() {
  gvec_t vv = gvec_create(10, gtype_free_ivec_ref);
  gvec_append(vv, gtype_v(cgraph_ivec_create_ref()));
  gvec_append(vv, gtype_v(cgraph_ivec_create_ref()));
  gvec_append(vv, gtype_v(cgraph_ivec_create_ref()));
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 5; ++j) {
      cgraph_ivec_push_back(
          gtype_ivec_ref(gvec_elem(vv, i)), (i + 1) * j);
    }
  }
  for (int i = 0; i < gvec_size(vv); ++i) {
    cgraph_ivec_print(*(gtype_ivec_ref(gvec_elem(vv, i))));
  }
  gvec_free(vv);
  return 0;
}