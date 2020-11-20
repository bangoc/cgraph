#ifndef TESTS_BASE_UT_H_
#define TESTS_BASE_UT_H_

#include "cgraph_vector.h"

cgraph_ivec_t cgraph_ivec_fromarray(CGRAPH_INTEGER *a, CGRAPH_INTEGER n);

bool cgraph_ivec_equal(cgraph_ivec_t v, CGRAPH_INTEGER *a);

#endif  // TESTS_BASE_UT_H_