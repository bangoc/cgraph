#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cgen/all.h"
#include "cgraph.h"

int main() {
  FILE *inp = fopen("input.txt", "r");
  int n, m;
  fscanf(inp, "%d%d", &n, &m);
  char buff[256];
  struct si2ways *conv = s2w_create();
  for (int i = 0; i < n; ++i) {
    fscanf(inp, "%s", buff);
    s2w_put(conv, buff);
  }

  char job1[256], job2[256];
  arr_make(edges, 0, CGRAPH_INTEGER);
  for (int i = 0; i < m; ++i) {
    fscanf(inp, "%s%s", job1, job2);
    int j1 = s2w_id(conv, job1),
        j2 = s2w_id(conv, job2);
    arr_append(edges, j1);
    arr_append(edges, j2);
  }

  cgraph_t g = cgraph_create(edges, 0, true);
  atype(CGRAPH_INTEGER) *order = cgraph_topological_sorting(g, CGRAPH_OUT);
  FILE *out = fopen("output.txt", "w");
  if (arr_size(order) < n) {
    fprintf(out, "-1");
  } else {
    for (int i = 0; i < arr_size(order); ++i) {
      fprintf(out, "%s\n", s2w_str(conv, i));
    }
  }
  fclose(out);
  cgraph_destroy(g);
  arr_free(edges);
  arr_free(order);
  s2w_free(conv);
  fclose(inp);
  return 0;
}