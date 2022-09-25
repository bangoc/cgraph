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
  cgraph_ivec_t edges = cgraph_ivec_create();
  for (int i = 0; i < m; ++i) {
    fscanf(inp, "%s%s", job1, job2);
    int j1 = s2w_id(conv, job1),
        j2 = s2w_id(conv, job2);
    cgraph_ivec_push_back(&edges, j1);
    cgraph_ivec_push_back(&edges, j2);
  }

  cgraph_t g = cgraph_create(edges, 0, true);
  cgraph_ivec_t order = cgraph_ivec_create();
  cgraph_topological_sorting(g, &order, CGRAPH_OUT);
  FILE *out = fopen("output.txt", "w");
  if (cgraph_ivec_size(order) < n) {
    fprintf(out, "-1");
  } else {
    for (int i = 0; i < cgraph_ivec_size(order); ++i) {
      fprintf(out, "%s\n", s2w_str(conv, i));
    }
  }
  fclose(out);
  cgraph_destroy(&g);
  cgraph_ivec_free(&edges);
  cgraph_ivec_free(&order);
  s2w_free(conv);
  fclose(inp);
  return 0;
}