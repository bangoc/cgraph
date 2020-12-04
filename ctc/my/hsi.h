#ifndef CTC_MY_HSI_H_
#define CTC_MY_HSI_H_

#include "ctc/hashtable.h"

typedef HashTable *HSI;

HSI hsi_create();
void hsi_free(HSI *tbl);

int hsi_add(HSI tbl, char *key, int val);
int hsi_get(HSI tbl, char *key, int **out);
int hsi_contains(HSI tbl, char *key);

#endif  // CTC_MY_SI_H_