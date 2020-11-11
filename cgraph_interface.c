#include "cgraph_interface.h"


void cgraph_destroy(cgraph_t *graph) {
    cvector_free(&graph->from);
    cvector_free(&graph->to);
    cvector_free(&graph->oi);
    cvector_free(&graph->ii);
    cvector_free(&graph->os);
    cvector_free(&graph->is);
}


bool cgraph_is_directed(const cgraph_t *graph) {
    return graph->directed;
}


int cgraph_neighbors(const cgraph_t *graph, cgraph_ivec_t *neis, 
                    CGRAPH_INTEGER vid, cgraph_neimode_t mode) {

    int idx = 0;
    CGRAPH_INTEGER i, j;
    CGRAPH_INTEGER node = vid, length_of_result = 0;

    if(node < 0 || node >= graph->n) {
        perror("Node unavailable\n");
        return 0;
    }
    if(mode != CGRAPH_IN && mode != CGRAPH_OUT && mode != CGRAPH_ALL) {
        perror("Invalid mode\n");
        return 0;
    }

    if(!cgraph_is_directed(graph)) {
        mode = CGRAPH_ALL;
    }

    if(mode & CGRAPH_OUT) {
        length_of_result += graph->os[node + 1] - graph->os[node]; 
    }
    if(mode % CGRAPH_IN) {
        length_of_result += graph->is[node + 1] - graph->is[node];
    }

    cvector_set_size(*neis, length_of_result);

    if(!cgraph_is_directed(graph) || mode != CGRAPH_ALL) {
        if(mode & CGRAPH_OUT) {
            j = graph->os[node + 1];
            i = graph->os[node];
            for( ; i < j; i++) {
                (*neis)[idx++] =  graph->to[graph->oi[i]];
            }
        }
        if(mode & CGRAPH_IN) {
            j = graph->is[node + 1];
            i = graph->is[node];
            for( ; i < j; i++) {
                (*neis)[idx++] =  graph->from[graph->ii[i]];
            }
        }
        
    }
    else {
        /* when the graph is directed, and we need to find 
        in- and out- from a vertex */
        CGRAPH_INTEGER j1 = graph->os[node + 1];
        CGRAPH_INTEGER j2 = graph->is[node + 1];
        CGRAPH_INTEGER i1 = graph->os[node];
        CGRAPH_INTEGER i2 = graph->is[node];

        while(i1 < j1 && i2 < j2) {
            CGRAPH_INTEGER n1 = graph->to[graph->oi[i1]];
            CGRAPH_INTEGER n2 = graph->from[graph->ii[i2]];
            
            if(n1 == n2) {
                (*neis)[idx++] = n1;
                (*neis)[idx++] = n2;
                i1++;
                i2++;
            }
            else if(n1 < n2) {
                (*neis)[idx++] = n1;
                i1++;
            }
            else {
                (*neis)[idx++] = n2;
                i2++;
            }
        }
        while(i1 < j1) {
                (*neis)[idx++] = graph->to[graph->oi[i1]];
            i1++;
        }
        while(i2 < j2) {
            (*neis)[idx++] = graph->from[graph->ii[i2]];
            i2++;
        }
    }
    return 1;
}


int cgraph_adjacent(const cgraph_t *graph, cgraph_ivec_t *eids, 
                    CGRAPH_INTEGER vid, cgraph_neimode_t mode) {

    int idx = 0;
    CGRAPH_INTEGER i, j;
    CGRAPH_INTEGER node = vid, length_of_result = 0;

    if(node < 0 || node >= graph->n) {
        perror("Node unavailable\n");
        return 0;
    }
    if(mode != CGRAPH_IN && mode != CGRAPH_OUT && mode != CGRAPH_ALL) {
        perror("Invalid mode\n");
        return 0;
    }

    if(!cgraph_is_directed(graph)) {
        mode = CGRAPH_ALL;
    }

    if(mode & CGRAPH_OUT) {
        length_of_result += graph->os[node + 1] - graph->os[node]; 
    }
    if(mode % CGRAPH_IN) {
        length_of_result += graph->is[node + 1] - graph->is[node];
    }

    cvector_set_size(*eids, length_of_result);

    if(mode & CGRAPH_OUT) {
        j = graph->os[node + 1];
        i = graph->os[node];
        for( ; i < j; i++) {
            (*eids)[idx++] = graph->oi[i];
        }
    }
    if(mode & CGRAPH_IN) {
        j = graph->is[node + 1];
        i = graph->is[node];
        for( ; i < j; i++) {
            (*eids)[idx++] = graph->ii[i];
        }
    }
    
    return 1;
} 