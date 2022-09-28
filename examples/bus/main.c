#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "cgen/all.h"
#include "cgraph.h"

typedef struct bus_stop {
  long bus;
  long stop;
} *bus_stop_t;

hmap_t stop_id = NULL;
gvec_t id_stop = NULL;
gvec_t stop_buses = NULL;
hmap_t bus_id = NULL;
gvec_t id_bus = NULL;

gvec_t nodes = NULL;
atype(CGRAPH_INTEGER) *edges = NULL;
atype(CGRAPH_REAL) *weights = NULL;

long k_cost_change_bus = 1,
     k_cost_nex_stop = 1000;

void init_global() {
  stop_id = hmap_create(gtype_hash_s, gtype_cmp_s, NULL, NULL);
  id_stop = gvec_create(0, gtype_free_s);
  stop_buses = gvec_create(0, gtype_free_iarr_ref);
  bus_id = hmap_create(gtype_hash_s, gtype_cmp_s, NULL, NULL);
  id_bus = gvec_create(5, gtype_free_s);
  nodes = gvec_create(5, gtype_free_v);
  edges = arr_create(0, CGRAPH_INTEGER);
  weights = arr_create(0, CGRAPH_REAL);
}

void free_global() {
  hmap_free(stop_id);
  gvec_free(id_stop);
  hmap_free(bus_id);
  gvec_free(id_bus);
  gvec_free(nodes);
  gvec_free(stop_buses);
  arr_free(edges);
  arr_free(weights);
}

long get_save_id(hmap_t si, gvec_t is, char *s) {
  // Xóa khoảng trắng ở đầu và cuối
  while (isspace(*s)) ++s;
  while (strlen(s) > 0 && isspace(s[strlen(s) - 1])) s[strlen(s) - 1] = '\0';

  gtype *id = hmap_value(si, gtype_s(s));
  if (id != NULL) {
    return id->l;
  }
  char *str = strdup(s);
  gvec_append(is, gtype_s(str));
  long tmp = gvec_size(is) - 1;
  hmap_insert(si, gtype_s(str), gtype_l(tmp));
  return tmp;
}

void parse_input(char *fname) {
  FILE *inp = fopen(fname, "r");
  long n = 0;
  char *line = NULL;
  while (cgetline(&line, &n, inp) > 0) {
    char *cur = strchr(line, ':');
    if (!cur) {
      continue;
    }
    *cur = '\0';
    long id1 = get_save_id(bus_id, id_bus, line);
    const char *delims = ">";
    char *stop = strtok(cur + 1, delims);
    bool first = true;
    while (stop) {
      long id2 = get_save_id(stop_id, id_stop, stop);
      bus_stop_t tmp = malloc(sizeof(struct bus_stop));
      tmp->bus = id1;
      tmp->stop = id2;
      gvec_append(nodes, gtype_v(tmp));
      if (!first) {
        long tmp = gvec_size(nodes) - 1;
        arr_append(edges, tmp - 1);
        arr_append(edges, tmp);
        arr_append(weights, k_cost_nex_stop);
      }
      first = false;
      if (id2 >= gvec_size(stop_buses)) {
        gvec_append(stop_buses, gtype_v(arr_icreate_ref()));
      }
      arr_append(arr_iptr_at(stop_buses, id2), gvec_size(nodes) - 1);
      stop = strtok(NULL, delims);
    }
  }
  fclose(inp);
  free(line);
}

void bus_change() {
  for (int i = 0; i < gvec_size(stop_buses); ++i) {
    atype(CGRAPH_INTEGER) *v = *((atype(CGRAPH_INTEGER) **)(gvec_elem(stop_buses, i).v));
    long sz = gvec_size(nodes);
    for (int j = 0; j < arr_size(v); ++j) {
      arr_append(edges, sz + 2 * i);
      arr_append(edges, v[j]);
      arr_append(edges, v[j]);
      arr_append(edges, sz + 2 * i + 1);
      arr_append(weights, k_cost_change_bus);
      arr_append(weights, k_cost_change_bus);
    }
    for (int i = 0; i < arr_size(v) - 1; ++i) {
      for (int j = i + 1; j < arr_size(v); ++j) {
        arr_append(edges, v[i]);
        arr_append(edges, v[j]);
        arr_append(edges, v[j]);
        arr_append(edges, v[i]);
        arr_append(weights, k_cost_change_bus);
        arr_append(weights, k_cost_change_bus);
      }
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: ./bus input.txt\n");
    return 1;
  }
  init_global();
  parse_input(argv[1]);
  bus_change();
  cgraph_t g = cgraph_create(edges, 0, true);
  char *beg = NULL, *end = NULL;
  long beg_len = 0, end_len = 0;
  printf("Nhập điểm bắt đầu: ");
  remove_tail_lf(cgetline(&beg, &beg_len, stdin));
  printf("Nhập điểm kết thúc: ");
  remove_tail_lf(cgetline(&end, &end_len, stdin));
  printf("Từ %s tới %s: \n", beg, end);
  gtype *beg_id = hmap_value(stop_id, gtype_s(beg)),
        *end_id = hmap_value(stop_id, gtype_s(end));
  if (beg_id == NULL || end_id == NULL) {
    printf("Invalid stop\n");
  } else {
    long from = gvec_size(nodes) + 2 * beg_id->l,
         to = gvec_size(nodes) + 2 * end_id->l + 1;
    struct path *p = cgraph_get_shortest_path_dijkstra(g, from, to, weights, CGRAPH_OUT);
    if (p->reached) {
      printf("Found path: \n");
      for (int i = 1; i < arr_size(p->vertices) - 1; ++i) {
        long bus = ((bus_stop_t)(gvec_elem(nodes, p->vertices[i]).v))->bus,
             stop = ((bus_stop_t)(gvec_elem(nodes, p->vertices[i]).v))->stop;
        printf("%s(%s)\n", gvec_elem(id_stop, stop).s, gvec_elem(id_bus, bus).s);
      }
    } else {
      printf("Path not found.\n");
    }
    free_path(p);
  }
  free_global();
  free(beg);
  free(end);
  cgraph_destroy(g);
  return 0;
}
