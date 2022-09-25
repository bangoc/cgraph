#ifndef CGRAPH_ARR_H_
#define CGRAPH_ARR_H_

/* (C) Nguyen Ba Ngoc 2022 */

/** @file
 * @brief Các triển khai riêng của arr cho số nguyên và số thực để
 * sử dụng trong cgraph
 */

#include "cgen/vec/arr.h"
#include "cgraph_datatype.h"

CGRAPH_INTEGER arr_imax(arr_ptr(CGRAPH_INTEGER) a);

int arr_irange(arr_ptr(CGRAPH_INTEGER) a,
               CGRAPH_INTEGER low, CGRAPH_INTEGER high);

arr_ptr(CGRAPH_INTEGER) arr_ifrom_array(CGRAPH_INTEGER *a, CGRAPH_INTEGER n);

#endif  // CGRAPH_ARR_H_