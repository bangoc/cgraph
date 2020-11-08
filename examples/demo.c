#include<stdio.h>

#include "cgraph.h"


void print_vector(cvector_vector_type(int) v) {
	printf("v(%lu): {", cvector_size(v));
	for (int i = 0; i < cvector_size(v); ++i) {
		if (i == 0) {
			printf("%d", v[i]);
		} 
		else {
			printf(", %d", v[i]);
		}
	}
	printf("}\n");
}

int main()
{
    cgraph_t g;
    cgraph_ivec_t v = NULL;
    int push;
    FILE * a = fopen("input.txt", "r");
    while(fscanf(a, "%d", &push) != EOF) {
        cvector_push_back(v, push);
    }
    fclose(a);

    cgraph_create(&g, v, 0, false);
    
    print_vector(g.from);
    print_vector(g.to);
    print_vector(g.oi);
    print_vector(g.ii);
    print_vector(g.os);
    print_vector(g.is);
    // cgraph_destroy(&g);
    return 0;
}