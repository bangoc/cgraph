#include <stdlib.h>
#include <string.h>

#include "hsi.h"

hsi_t hsi_create() {
  hsi_t tbl = hmap_create(gtype_hash_s, gtype_cmp_s, gtype_free_s, NULL);
  return tbl;
}

void hsi_free(hsi_t tbl) {
  hmap_free(tbl);
}

int hsi_add(hsi_t tbl, const char *key, int val) {
  char *k = strdup(key);
  hmap_ires res = hmap_insert(tbl, gtype_s(k), gtype_l(val));
  if (res.inserted) {
    return 0;
  }
  free(k);
  return 1;
}

int hsi_get(hsi_t tbl, const char *key, int *out) {
  gtype *tmp = hmap_value(tbl, gtype_s(key));
  if (!tmp) {
    return 1;
  }
  *out = tmp->l;
  return 0;
}

int hsi_contains(hsi_t tbl, const char *key) {
  return hmap_value(tbl, gtype_s(key)) != NULL;
}

void hsi_foreach_kv(hsi_t tbl,
                    int (*op)(const char*, int, void*),
                    void *user_data) {
  hmap_traverse(k, v, tbl) {
    if (op(k->s, v->l, user_data)) {
      break;
    }
  }
}