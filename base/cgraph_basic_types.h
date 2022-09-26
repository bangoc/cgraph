#ifndef BASE_CGRAPH_BASIC_TYPES_H
#define BASE_CGRAPH_BASIC_TYPES_H

#include <inttypes.h>
#include <stdbool.h>

#ifndef CGRAPH_INTEGER
#define CGRAPH_INTEGER int32_t
#endif

#ifndef CGRAPH_REAL
#define CGRAPH_REAL double
#endif

#if defined(NAN)
    #define CGRAPH_NAN NAN
#elif defined(INFINITY)
    #define CGRAPH_NAN (INFINITY/INFINITY)
#else
    #define CGRAPH_NAN (INT_LEAST32_MIN)
#endif

#endif  // BASE_CGRAPH_BASIC_TYPES_H