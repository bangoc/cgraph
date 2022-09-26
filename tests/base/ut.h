#ifndef TESTS_BASE_UT_H_
#define TESTS_BASE_UT_H_

#include "cgraph.h"

bool cgraph_ivec_equal(cgraph_ivec_t v, CGRAPH_INTEGER *a, CGRAPH_INTEGER n);

int arr_iequal(arr_ptr(CGRAPH_INTEGER) v, CGRAPH_INTEGER *a, long n);

void print_ivec(cgraph_ivec_t const v);

#define UT_MSG_FAILED(msg) \
    do { \
      printf("%s: Failed\n", msg); \
    } while (0)

#define UT_MSG_OK(msg) \
    do { \
      printf("%s: Ok\n", msg); \
    } while (0)

bool ut_is_connected(cgraph_t g, CGRAPH_INTEGER v1, CGRAPH_INTEGER v2,
        bool directed);

#endif  // TESTS_BASE_UT_H_