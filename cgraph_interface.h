#ifndef CGRAPH_INTERFACE_H_
#define CGRAPH_INTERFACE_H_

#include "cgraph_datatype.h"
#include "cgraph_constants.h"
#include <stdio.h> // printf, perror

/** 
 * @brief free all memory.
 * @param graph the graph being executed
 * @return void
 */
void cgraph_destroy(cgraph_t *graph);

/** 
 * @brief cgraph_is_directed.
 * @param graph the graph being executed
 * @return true if the graph is directed,
 * false if indirected.
 */
bool cgraph_is_directed(const cgraph_t *graph);

/**
 * @brief find all neighbors of a node.
 * @param graph the graph being executed.
 * @param neis the result vector, containing 
 * every neighbor nodes.
 * @param vid the node for which its 
 * neighbor nodes are searched.
 * @param mode CGRAPH_OUT = 1, find nodes 
 * to which \c vid is reachable,
 * CGRAPH_IN = 2, find nodes from which 
 * \c vid is reachable, CGRAPH_ALL = 3, both 1 and 2.
 * @return 1 if successful, 0 if not
 */
int cgraph_neighbors(const cgraph_t *graph, cgraph_ivec_t *neis, 
                     CGRAPH_INTEGER vid, cgraph_neimode_t mode);

/**
 * @brief find all neighbors of a node.
 * @param graph the graph being executed.
 * @param eids the result vector, containing 
 * every adjacent index.
 * @param vid the node for which its 
 * adjacent index are searched.
 * @param mode CGRAPH_OUT = 1, find nodes 
 * to which \c vid is reachable,
 * CGRAPH_IN = 2, find nodes from which 
 * \c vid is reachable, CGRAPH_ALL = 3, both 1 and 2.
 * @return 1 if successful, 0 if not
 */                     
int cgraph_adjacent(const cgraph_t *graph, cgraph_ivec_t *eids, 
                    CGRAPH_INTEGER vid, cgraph_neimode_t mode); 


#endif  // CGRAPH_INTERFACE_H_