#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "cgraph.h"

int main() {
  std::ifstream inp("input.txt");
  int n, m;
  inp >> n >> m;
  std::string buff;
  int id = 0;
  std::map<std::string, int> name_to_id;
  std::vector<std::string> id_to_name(n);
  for (int i = 0; i < n; ++i) {
    inp >> buff;
    name_to_id[buff] = id;
    id_to_name[id++] = buff;
  }
  std::string job1, job2;
  cgraph_ivec_t edges = cgraph_ivec_create();
  for (int i = 0; i < m; ++i) {
    inp >> job1 >> job2;
    cgraph_ivec_push_back(&edges, name_to_id[job1]);
    cgraph_ivec_push_back(&edges, name_to_id[job2]);
  }

  cgraph_t g;
  cgraph_create(&g, edges, 0, true);
  cgraph_ivec_t order = cgraph_ivec_create();
  cgraph_topological_sorting(&g, &order, CGRAPH_OUT);
  std::ofstream out("output.txt");
  if (cgraph_ivec_size(order) < n) {
    out << "-1" << std::endl;
  } else {
    for (int i = 0; i < cgraph_ivec_size(order); ++i) {
      out << id_to_name[ order[i] ] << std::endl;
    }
  }
  cgraph_ivec_free(&edges);
  cgraph_ivec_free(&order);
  cgraph_destroy(&g);
  return 0;
}