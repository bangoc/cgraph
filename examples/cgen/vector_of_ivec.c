#include "cgen/gtvector.h"
#include "cgraph_ivec.h"

int main() {
  vector_t vv = gtv_create();
  gtv_push_back(&vv, (gtype){.v = cgraph_ivec_create()});
  gtv_push_back(&vv, (gtype){.v = cgraph_ivec_create()});
  gtv_push_back(&vv, (gtype){.v = cgraph_ivec_create()});
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 5; ++j) {
      cgraph_ivec_push_back(
          gtv_ref_at(vv, i, cgraph_ivec_t *), (i + 1) * j);
    }
  }
  for (int i = 0; i < gtv_size(vv); ++i) {
    cgraph_ivec_print(gtv_val_at(vv, i, cgraph_ivec_t));
  }
  for (int i = 0; i < gtv_size(vv); ++i) {
    cgraph_ivec_free(gtv_ref_at(vv, i, cgraph_ivec_t *));
  }
  gtv_free(&vv);
  return 0;
}