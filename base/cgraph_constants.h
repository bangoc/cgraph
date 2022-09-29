#ifndef BASE_CGRAPH_CONSTANTS_H_
#define BASE_CGRAPH_CONSTANTS_H_

#include <inttypes.h>

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

#if defined(NAN)
    #define CGRAPH_NAN NAN
#elif defined(INFINITY)
    #define CGRAPH_NAN (INFINITY/INFINITY)
#else
    #define CGRAPH_NAN (INT_LEAST32_MIN)
#endif

#endif  // BASE_CGRAPH_CONSTANTS_H_