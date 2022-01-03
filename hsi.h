#ifndef HSI_H_
#define HSI_H_

#include "cgen/cgen.h"

typedef hmap_t hsi_t;

hsi_t hsi_create();
void hsi_free(hsi_t tbl);

int hsi_add(hsi_t tbl, const char *key, int val);
int hsi_get(hsi_t tbl, const char *key, int *out);
int hsi_contains(hsi_t tbl, const char *key);

void hsi_foreach_kv(hsi_t tbl,
                    int (*op)(const char*, int, void*),
                    void *user_data);

#endif  // HSI_H_