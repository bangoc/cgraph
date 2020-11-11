#include<stdio.h>

#include "cgraph.h"


void print_vector(cvector_vector_type(CGRAPH_INTEGER) v) {
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
    cgraph_ivec_t v = cvector_create_empty();
    int push;
    FILE * a = fopen("input.txt", "r");
    while(fscanf(a, "%d", &push) != EOF) {
        cvector_push_back(v, push);
    }
    fclose(a);

    cgraph_create(&g, v, 0, true);

    // print_vector(g.from);
    // print_vector(g.to);
    // print_vector(g.oi);
    // print_vector(g.ii);
    // print_vector(g.os);
    // print_vector(g.is);

    cgraph_ivec_t neis = cvector_create_empty();
    cgraph_neighbors(&g, &neis, 1, CGRAPH_ALL);
    print_vector(neis);

    cgraph_ivec_t adj = cvector_create_empty();
    cgraph_adjacent(&g, &adj, 1, CGRAPH_ALL);
    print_vector(adj);

    cvector_free(v);
    cvector_free(&neis);
    cvector_free(&adj);
    cgraph_destroy(&g);
    printf("free success");
    return 0;
}