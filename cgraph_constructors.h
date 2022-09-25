#ifndef CGRAPH_CONSTRUCTORS_H_
#define CGRAPH_CONSTRUCTORS_H_

/* (C) Nguyen Ba Ngoc 2021-2022 */

#include "cgen/vec/arr.h"
#include "cgraph_datatype.h"

#ifdef __cplusplus
extern "C"{
#endif

cgraph_t cgraph_create_empty(CGRAPH_INTEGER n, bool directed);

cgraph_t cgraph_create(arr_ptr(CGRAPH_INTEGER) edges,
                  CGRAPH_INTEGER n,
                  bool directed);
void cgraph_print(cgraph_t const g);

#ifdef __cplusplus
}
#endif

#endif  // CGRAPH_CONSTRUCTORS_H_