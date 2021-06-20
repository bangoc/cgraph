#include "cgraph.h"
#include "cgraph_error.h"
#include <stdio.h>

int main()
{
	cgraph_set_error_handler(cgraph_error_print);
	cgraph_ivec_t edges = cgraph_ivec_create();
	
	cgraph_ivec_push_back(&edges, 1);
	cgraph_ivec_push_back(&edges, 2);

   	cgraph_t g = cgraph_create(edges, 0, CGRAPH_UNDIRECTED);
   	//In lỗi thêm đỉnh âm
	cgraph_add_vertices(g,-1);
	//IN lỗi tạo đồ thi với số đỉnh âm
	cgraph_create_empty(-1, true);
	//IN lỗi topological sorting với đồ thị vô hướng
	cgraph_topological_sorting(g,
                               NULL,
                               CGRAPH_ALL);

	
	return 0;
}