#ifndef CGRAPH_CONSTRUCTORS_H_
#define CGRAPH_CONSTRUCTORS_H_

#include "cgraph_datatype.h"
#include <stdio.h>  // for printf
#include <string.h> // for memset

typedef struct att {
    int from;
    int to;
    int index;
} attribute;


int cgraph_create(cgraph_t *graph, cgraph_ivec_t edges, 
                    CGRAPH_INTEGER n, bool directed);


int cmpFrom(const void * a, const void * b);

int cmpTo(const void * a, const void * b);

void new_from_to(cgraph_t *graph, cgraph_ivec_t edges);

cvector_vector_type(attribute) create_temporary_vector(cgraph_t *graph, int N);

int number_vertices(cgraph_ivec_t edges);

int binarySearch_leftmost(cvector_vector_type(attribute) v, 
                            int key, int mode);


void new_from_to(cgraph_t *graph, cgraph_ivec_t edges);


void new_oi(cgraph_t *graph, cvector_vector_type(attribute) v, 
                            int num_vertices);


void new_os(cgraph_t * graph, cvector_vector_type(attribute) v, 
                            int num_vertices);

void new_ii(cgraph_t *graph, cvector_vector_type(attribute) v, 
                            int num_vertices);

void new_is(cgraph_t * graph, cvector_vector_type(attribute) v, 
                            int num_vertices);



#endif  // CGRAPH_CONSTRUCTORS_H_