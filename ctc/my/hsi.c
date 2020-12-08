#include <stdlib.h>
#include <string.h>

#include "hsi.h"

HSI hsi_create() {
  HSI tbl;
  hashtable_new(&tbl);
  return tbl;
}

void hsi_free(HSI *tbl) {
  hashtable_destroy(*tbl);
}

int hsi_add(HSI tbl, char *key, int val) {
  int *data = malloc(sizeof(data));
  *data = val;
  if (hashtable_add(tbl, strdup(key), data) == CC_OK) {
    return 0;
  }
  return 1;
}

int hsi_get(HSI tbl, char *key, int **out) {
  void *data;
  if (hashtable_get(tbl, key, &data) == CC_OK) {
    *out = (int*)data;
    return 0;
  }
  return 1;
}

int hsi_contains(HSI tbl, char *key) {
  return hashtable_contains_key(tbl, key);
}

void hsi_foreach_kv(HSI tbl,
                    void (*op)(const char*, int*, void*),
                    void *user_data) {
  void *key;
  void *value;
  HASHTABLE_FOREACH(tbl, key, value, op(key, value, user_data););
}