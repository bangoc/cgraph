#ifndef CGRAPH_CONSTANTS_H_
#define CGRAPH_CONSTANTS_H_

typedef enum {
  CGRAPH_OUT = 1,
  CGRAPH_IN = 2,
  CGRAPH_ALL = 3,
  CGRAPH_TOTAL = 3
} cgraph_neimode_t;

typedef enum {
  CGRAPH_UNDIRECTED = 0,
  CGRAPH_DIRECTED = 1
} cgraph_i_directed_t;

#endif  // CGRAPH_CONSTANTS_H_