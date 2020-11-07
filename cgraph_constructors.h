#ifndef CGRAPH_CONSTRUCTORS_H_
#define CGRAPH_CONSTRUCTORS_H_

#include "cgraph_datatype.h"

typedef struct att {
    int from;
    int to;
    int index;
} attribute;

void print_vector(cgraph_ivec_t v) {
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

int cmpFrom(const void * a, const void * b) {
    int x = ((attribute *) a)->from;
    int y = ((attribute *) b)->from;
    return x - y;
}

int cmpTo(const void * a, const void * b) {
    int x = ((attribute *) a)->to;
    int y = ((attribute *) b)->to;
    return x - y;
}

int cgraph_create(cgraph_t *graph, cgraph_ivec_t edges, CGRAPH_INTEGER n, bool directed) {
    int N = (int) cvector_size(edges);

    graph->from = NULL;
    graph->to = NULL;
    graph->oi = NULL;
    graph->ii = NULL;
    graph->os = NULL;
    graph->is = NULL;
    /* finding graph->from and graph->to */
    for(int i = 0; i < N; i++) {
        if(i % 2 == 0) {
            cvector_push_back(graph->from, edges[i]);
        }
        else {
            cvector_push_back(graph->to, edges[i]);
        }
    }

    /* using v as a temporary vector to sort */
    cvector_vector_type(attribute) v = NULL;
    for(int i = 0; i < N / 2; i++) {
        attribute f;
        f.from = graph->from[i];
        f.to = graph->to[i];
        f.index = i;
        cvector_push_back(v, f);
    }
    qsort(v, N / 2, sizeof(attribute), cmpFrom);        // this one to find graph->os and graph->oi
    /* finding graph->os */
    int start = 0, end = 0;
    for(; end < N / 2; end++) {
        if(v[end].from != v[start].from) {
            cvector_push_back(graph->os, start);
            start = end;
        }
    }
    cvector_push_back(graph->os, start);
    /* finding graph->oi */
    end = 0;
    for(; end < (int) cvector_size(graph->os); end++) {
        if(end == (int) cvector_size(graph->os) - 1) {
            qsort(v + graph->os[end], N / 2 - graph->os[end], sizeof(attribute), cmpTo);
        }
        else {
            qsort(v + graph->os[end], graph->os[end+1] - graph->os[end], sizeof(attribute), cmpTo);
        }
    }
    for(int i = 0; i < N / 2; i++) {
        cvector_push_back(graph->oi, v[i].index);
    }
    print_vector(graph->oi);
    print_vector(graph->os);
}

#endif  // CGRAPH_CONSTRUCTORS_H_