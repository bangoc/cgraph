#include "cgraph.h"
#include "cgraph_error.h"
#include <stdio.h>

int main()
{
	cgraph_set_error_handler(cgraph_error_print); //Thiết lập hàm xử lý ngoại lệ
	cgraph_ivec_t edges = cgraph_ivec_create();
	
	cgraph_ivec_push_back(&edges, 1);
	cgraph_ivec_push_back(&edges, 2);

   	cgraph_t g = cgraph_create(edges, 0, CGRAPH_UNDIRECTED);
   	//Gọi hàm cgraph_add_vertices với lỗi tiền điều kiện: số đỉnh thêm âm
	cgraph_add_vertices(g,-1);
	//Gọi hàm cgraph_create_empty với lỗi tiền điều kiện: số đỉnh âm
	cgraph_create_empty(-1, true);
	//Gọi hàm cgraph_topological_sorting với lỗi tiền điều kiện: đồ thị vô hướng
	cgraph_topological_sorting(g,
                               NULL,
                               CGRAPH_ALL);
	return 0;
}