#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "cgen/gtvector.h"
#include "cgen/s2i.h"
#include "cgen/svector.h"
#include "cgraph.h"
#include "getline.h"

typedef struct bus_stop {
  long bus;
  long stop;
} *bus_stop_t;

bn_tree_t stop_id = NULL_PTR;
vector_t id_stop = NULL_PTR;
vector_t stop_buses = NULL_PTR;
bn_tree_t bus_id = NULL_PTR;
vector_t id_bus = NULL_PTR;

vector_t nodes = NULL_PTR;
cgraph_ivec_t edges = NULL_PTR;
cgraph_rvec_t weights = NULL_PTR;

long k_cost_change_bus = 1,
     k_cost_nex_stop = 1000;

void init_global() {
  stop_id = s2i_create();
  id_stop = gtv_create();
  stop_buses = gtv_create();
  bus_id = s2i_create();
  id_bus = gtv_create();
  nodes = gtv_create();
  edges = cgraph_ivec_create();
  weights = cgraph_rvec_create();
}

void free_global() {
  s2i_free(&stop_id);
  svec_free(&id_stop);
  s2i_free(&bus_id);
  svec_free(&id_bus);
  cgraph_ivec_free(&edges);
  cgraph_rvec_free(&weights);
  for (int i = 0; i < gtv_size(nodes); ++i) {
    free(nodes[i].v);
  }
  gtv_free(&nodes);
  for (int i = 0; i < gtv_size(stop_buses); ++i) {
    cgraph_ivec_t v = stop_buses[i].v;
    cgraph_ivec_free(&v);
  }
  gtv_free(&stop_buses);
}

long get_save_id(bn_tree_t si, vector_t *is, char *s) {
  long id = s2i_value(si, s);
  if (id != k_s2i_invalid) {
    return id;
  }
  svec_push_back(is, s);
  id = gtv_size(*is) - 1;
  s2i_insert(si, s, id);
  return id;
}

void parse_input(char *fname) {
  FILE *inp = fopen(fname, "r");
  long n = 0;
  char *line = NULL_PTR;
  while (my_getline(&line, &n, inp) > 0) {
    char *cur = strchr(line, ':');
    if (!cur) {
      continue;
    }
    *cur = '\0';
    long id1 = get_save_id(bus_id, &id_bus, line);
    const char *delims = ">";
    char *stop = strtok(cur + 1, delims);
    bool first = true;
    while (stop) {
      long id2 = get_save_id(stop_id, &id_stop, stop);
      bus_stop_t tmp = malloc(sizeof(struct bus_stop));
      tmp->bus = id1;
      tmp->stop = id2;
      gtv_push_back(&nodes, (gtype){.v = tmp});
      if (!first) {
        long tmp = gtv_size(nodes) - 1;
        cgraph_ivec_push_back(&edges, tmp - 1);
        cgraph_ivec_push_back(&edges, tmp);
        cgraph_rvec_push_back(&weights, k_cost_nex_stop);
      }
      first = false;
      if (id2 >= gtv_size(stop_buses)) {
        gtv_push_back(&stop_buses, (gtype){.v = cgraph_ivec_create()});
      }
      cgraph_ivec_push_back(&(stop_buses[id2].v), gtv_size(nodes) - 1);
      stop = strtok(NULL, delims);
    }
  }
  fclose(inp);
  free(line);
}

void bus_change() {
  for (int i = 0; i < gtv_size(stop_buses); ++i) {
    cgraph_ivec_t v = stop_buses[i].v;
    long sz = gtv_size(nodes);
    for (int j = 0; j < cgraph_ivec_size(v); ++j) {
      cgraph_ivec_push_back(&edges, sz + 2 * i);
      cgraph_ivec_push_back(&edges, v[j]);
      cgraph_ivec_push_back(&edges, v[j]);
      cgraph_ivec_push_back(&edges, sz + 2 * i + 1);
      cgraph_rvec_push_back(&weights, k_cost_change_bus);
      cgraph_rvec_push_back(&weights, k_cost_change_bus);
    }
    for (int i = 0; i < cgraph_ivec_size(v) - 1; ++i) {
      for (int j = i + 1; j < cgraph_ivec_size(v); ++j) {
        cgraph_ivec_push_back(&edges, v[i]);
        cgraph_ivec_push_back(&edges, v[j]);
        cgraph_ivec_push_back(&edges, v[j]);
        cgraph_ivec_push_back(&edges, v[i]);
        cgraph_rvec_push_back(&weights, k_cost_change_bus);
        cgraph_rvec_push_back(&weights, k_cost_change_bus);
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
  // cgraph_ivec_print(edges);
  cgraph_t g = cgraph_create(edges, 0, true);
  char *beg = NULL_PTR, *end = NULL_PTR;
  long beg_len = 0, end_len = 0;
  printf("Nhập điểm bắt đầu: ");
  my_getline(&beg, &beg_len, stdin);
  printf("Nhập điểm kết thúc: ");
  my_getline(&end, &end_len, stdin);
  printf("Từ %s tới %s: \n", beg, end);
  long beg_id = s2i_value(stop_id, beg),
       end_id = s2i_value(stop_id, end);
  if (beg_id == k_s2i_invalid || end_id == k_s2i_invalid) {
    printf("Invalid stop\n");
  } else {
    long from = gtv_size(nodes) + 2 * beg_id,
         to = gtv_size(nodes) + 2 * end_id + 1;
    cgraph_ivec_t vpath = cgraph_ivec_create(),
                  epath = cgraph_ivec_create();
    int ret = cgraph_get_shortest_path_dijkstra(g, &vpath, &epath, from, to, weights, CGRAPH_OUT);
    if (ret == 0) {
      printf("Found path: \n");
      for (int i = 1; i < cgraph_ivec_size(vpath) - 1; ++i) {
        long bus = ((bus_stop_t)nodes[vpath[i]].v)->bus,
             stop = ((bus_stop_t)nodes[vpath[i]].v)->stop;
        printf("%s(%s)\n", id_stop[stop].s, id_bus[bus].s);
      }
    } else {
      printf("Path not found.\n");
    }
  }
  free_global();
  free(beg);
  free(end);
  cgraph_destroy(&g);
  return 0;
}