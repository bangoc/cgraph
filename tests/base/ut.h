#ifndef TESTS_BASE_UT_H_
#define TESTS_BASE_UT_H_

#include "cgraph_ivec.h"

cgraph_ivec_t cgraph_ivec_fromarray(CGRAPH_INTEGER *a, CGRAPH_INTEGER n);

bool cgraph_ivec_equal(cgraph_ivec_t v, CGRAPH_INTEGER *a, CGRAPH_INTEGER n);

void print_ivec(cgraph_ivec_t const v);

#define UT_MSG_FAILED(msg) \
    do { \
      printf("%s: Failed\n", msg); \
    } while (0)

#define UT_MSG_OK(msg) \
    do { \
      printf("%s: Ok\n", msg); \
    } while (0)

#endif  // TESTS_BASE_UT_H_