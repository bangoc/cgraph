#include <stdio.h>
#include "ctc/my/hsi.h"

int main() {
  HSI tbl = hsi_create();
  hsi_add(tbl, "A", 0);
  hsi_add(tbl, "B", 1);
  hsi_add(tbl, "C", 2);
  if (hsi_contains(tbl, "A")) {
    printf("contains A\n");
  }
  if (hsi_contains(tbl, "D")) {
    printf("Suprised! contains D\n");
  } else {
    printf("Not contains D\n");
  }
  int *id;
  hsi_get(tbl, "C", &id);
  printf("Id of C is: %d\n", *id);
  hsi_free(&tbl);
  return 0; 
}