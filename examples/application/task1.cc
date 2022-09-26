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
  arr_make(edges, 0, CGRAPH_INTEGER);
  for (int i = 0; i < m; ++i) {
    inp >> job1 >> job2;
    arr_append(edges, name_to_id[job1]);
    arr_append(edges, name_to_id[job2]);
  }

  cgraph_t g = cgraph_create(edges, 0, true);
  arr_make(order, 0, CGRAPH_INTEGER);
  cgraph_topological_sorting(g, &order, CGRAPH_OUT);
  std::ofstream out("output.txt");
  if (arr_size(order) < n) {
    out << "-1" << std::endl;
  } else {
    for (int i = 0; i < arr_size(order); ++i) {
      out << id_to_name[ order[i] ] << std::endl;
    }
  }
  arr_free(edges);
  arr_free(order);
  cgraph_destroy(&g);
  return 0;
}