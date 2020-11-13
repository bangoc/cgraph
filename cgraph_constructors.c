#include "cgraph_constructors.h"



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


cvector_vector_type(attribute) create_temporary_vector(
                                    cgraph_t *graph, 
                                    int N) {

    cvector_vector_type(attribute) v = cvector_create_empty();
    for(int i = 0; i < N / 2; i++) {
        attribute f;
        f.from = graph->from[i];
        f.to = graph->to[i];
        f.index = i;
        cvector_push_back(v, f);
    }    
    return v;
}

int number_vertices(cgraph_ivec_t edges) {
    int N = cvector_size(edges);
    int max = 0;
    for(int i = 0; i < N; i++) {
        max = (max < edges[i] ? edges[i] : max);
    }
    return max + 1;
}


void new_from_to(cgraph_t *graph, cgraph_ivec_t edges) {
    int N = cvector_size(edges);
    for(int i = 0; i < N; i++) {
        if(i % 2 == 0) {
            cvector_push_back(graph->from, edges[i]);
        }
        else {
            cvector_push_back(graph->to, edges[i]);
        }
    }
}


void new_oi(cgraph_t *graph, cvector_vector_type(attribute) v, 
                            int num_vertices) {
    int N = cvector_size(v);
    qsort(v, N, sizeof(attribute), cmpFrom);

    int count[num_vertices];
    memset(count, 0, num_vertices * sizeof(int));
    for(int i = 0; i < N; i++) {
        count[v[i].from]++;
    }

    int start = 0;
    for(int i = 0; i < num_vertices; i++) {
        if(count[i] == 0) {
            continue;
        }
        qsort(v + start, count[i], sizeof(attribute), cmpTo);
        start += count[i];
    }
    for(int i = 0; i < N; i++) {
        cvector_push_back(graph->oi, v[i].index);
    }    
}


void new_os(cgraph_t * graph, cvector_vector_type(attribute) v, 
                            int num_vertices) {
    int N = cvector_size(v);

    qsort(v, N, sizeof(attribute), cmpFrom);

    for(int i = 0; i < v[0].from; i++) {
        cvector_push_back(graph->os, 0);
    }
    for(int i = 0; i < N; i++) {
        if(i == 0) {
            cvector_push_back(graph->os, 0);
        }
        else if(i >= 1 && v[i].from != v[i-1].from) {
            cvector_push_back(graph->os, i);
        }
    }
    for(int i = v[N-1].from + 1; i <= num_vertices; i++) {
        cvector_push_back(graph->os, N);
    }
}

void new_ii(cgraph_t *graph, cvector_vector_type(attribute) v, 
                            int num_vertices) {
    int N = cvector_size(v);
    qsort(v, N, sizeof(attribute), cmpTo);

    int count[num_vertices];
    memset(count, 0, num_vertices * sizeof(int));
    for(int i = 0; i < N; i++) {
        count[v[i].to]++;
    }

    int start = 0;
    for(int i = 0; i < num_vertices; i++) {
        if(count[i] == 0) {
            continue;
        }
        qsort(v + start, count[i], sizeof(attribute), cmpFrom);
        start += count[i];
    }
    for(int i = 0; i < N; i++) {
        cvector_push_back(graph->ii, v[i].index);
    }
}

void new_is(cgraph_t * graph, cvector_vector_type(attribute) v, 
                            int num_vertices) {
    int N = cvector_size(v);

    qsort(v, N, sizeof(attribute), cmpTo);

    for(int i = 0; i < v[0].to; i++) {
        cvector_push_back(graph->is, 0);
    }
    for(int i = 0; i < N; i++) {
        if(i == 0) {
            cvector_push_back(graph->is, 0);
        }
        else if(i >= 1 && v[i].to != v[i-1].to) {
            cvector_push_back(graph->is, i);
        }
    }
    for(int i = v[N-1].to + 1; i <= num_vertices; i++) {
        cvector_push_back(graph->is, N);
    }
}



int cgraph_create(cgraph_t *graph, cgraph_ivec_t edges, 
                    CGRAPH_INTEGER n, bool directed) {

    n = (CGRAPH_INTEGER) number_vertices(edges);
    int N = (int) cvector_size(edges);

    graph->n = n;
    graph->directed = directed;
    graph->from = NULL;
    graph->to = NULL;
    graph->oi = NULL;
    graph->ii = NULL;
    graph->os = NULL;
    graph->is = NULL;


    new_from_to(graph, edges);

    cvector_vector_type(attribute) v = create_temporary_vector(graph, N);

    new_oi(graph, v, n);
    new_os(graph, v, n);
    new_ii(graph, v, n);
    new_is(graph, v, n);

    cvector_free(v);
    return 0;
}