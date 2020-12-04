#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ctc/my/hsi.h"
#include "cgraph.h"

int main() {
  FILE *inp = fopen("input.txt", "r");
  int n, m;
  fscanf(inp, "%d%d", &n, &m);
  char buff[256];
  HSI name_to_id = hsi_create();
  int id = 0;
  char **id_to_name = calloc(n, sizeof(char*));
  for (int i = 0; i < n; ++i) {
    fscanf(inp, "%s", buff);
    hsi_add(name_to_id, buff, id);
    id_to_name[id++] = strdup(buff);
  }

  char job1[256], job2[256];
  cgraph_ivec_t edges = cgraph_ivec_create();
  for (int i = 0; i < m; ++i) {
    fscanf(inp, "%s %s", job1, job2);
    int *j1, *j2;
    hsi_get(name_to_id, job1, &j1);
    hsi_get(name_to_id, job2, &j2);
    cgraph_ivec_push_back(&edges, *j1);
    cgraph_ivec_push_back(&edges, *j2);
  }

  cgraph_t g;
  cgraph_create(&g, edges, 0, true);
  cgraph_ivec_t order = cgraph_ivec_create();
  cgraph_topological_sorting(&g, &order, CGRAPH_OUT);
  FILE *out = fopen("output.txt", "w");
  if (cgraph_ivec_size(order) < n) {
    fprintf(out, "-1");
  } else {
    for (int i = 0; i < cgraph_ivec_size(order); ++i) {
      fprintf(out, "%s\n", id_to_name[ order[i] ]);
    }
  }
  fclose(out);
  cgraph_destroy(&g);
  for (int i = 0; i < n; ++i) {
    free(id_to_name[i]);
  }
  free(id_to_name);
  cgraph_ivec_free(&edges);
  cgraph_ivec_free(&order);
  fclose(inp);
  return 0;
}