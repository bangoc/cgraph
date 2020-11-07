#include<stdio.h>

#include "cgraph.h"

// void print_vector(cgraph_ivec_t v) {
// 	printf("v(%lu): {", cvector_size(v));
// 	for (int i = 0; i < cvector_size(v); ++i) {
// 		if (i == 0) {
// 			printf("%d", v[i]);
// 		} 
// 		else {
// 			printf(", %d", v[i]);
// 		}
// 	}
// 	printf("}\n");
// }

int main()
{
    cgraph_t g;
    cgraph_ivec_t v = NULL;
    cvector_push_back(v, 0);
    cvector_push_back(v, 1);
    cvector_push_back(v, 0);
    cvector_push_back(v, 3);
    cvector_push_back(v, 1);
    cvector_push_back(v, 2);
    cvector_push_back(v, 1);
    cvector_push_back(v, 3);
    cvector_push_back(v, 2);
    cvector_push_back(v, 4);
    cvector_push_back(v, 0);
    cvector_push_back(v, 2);
    cvector_push_back(v, 3);
    cvector_push_back(v, 5);
    cvector_push_back(v, 3);
    cvector_push_back(v, 1);
    cvector_push_back(v, 1);
    cvector_push_back(v, 4);

    cgraph_create(&g, v, 0, true);
    // print_vector(g.from);
    return 0;
}