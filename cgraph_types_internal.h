#ifndef CGRAPH_TYPES_INTERNAL_H_
#define CGRAPH_TYPES_INTERNAL_H_

#include "cgraph_basic_types.h"
#include "cgraph_ivec.h"
#include "cgraph_rvec.h"

/* -------------------------------------------------- */
/* Cấu trúc heap được đánh chỉ mục 2 chiều             */
/* -------------------------------------------------- */

/* Đây là một cấu trúc heap được đánh chỉ mục 2 chiều. Ngoài các đặc
   tính của heap được đánh chỉ mục thông thường. Nó cho phép truy cập
   tới tất cả các phần tử của nó trong thời gian O(1). Nói cách khác,
   thao tác cập nhật giá trị cho cấu trúc heap này có độ phức tạp là
   O(1), còn đối với heap thường thao tác cập nhật được thực hiện
   với độ phức tạp O(n).... */

typedef struct cgraph_2wheap_t {
    CGRAPH_INTEGER size;
    cgraph_rvec_t data;
    cgraph_ivec_t index;
    cgraph_ivec_t index2;
} cgraph_2wheap_t;

#ifdef __cplusplus
extern "C"{
#endif

int cgraph_2wheap_init(cgraph_2wheap_t *h, CGRAPH_INTEGER size);
void cgraph_2wheap_free(cgraph_2wheap_t *h);
int cgraph_2wheap_clear(cgraph_2wheap_t *h);
int cgraph_2wheap_push_with_index(cgraph_2wheap_t *h,
                                  CGRAPH_INTEGER idx, CGRAPH_REAL elem);
bool cgraph_2wheap_empty(const cgraph_2wheap_t *h);
CGRAPH_INTEGER cgraph_2wheap_size(const cgraph_2wheap_t *h);
CGRAPH_INTEGER cgraph_2wheap_max_size(const cgraph_2wheap_t *h);
CGRAPH_REAL cgraph_2wheap_max(const cgraph_2wheap_t *h);
CGRAPH_INTEGER cgraph_2wheap_max_index(const cgraph_2wheap_t *h);
CGRAPH_REAL cgraph_2wheap_deactivate_max(cgraph_2wheap_t *h);
bool cgraph_2wheap_has_elem(const cgraph_2wheap_t *h, CGRAPH_INTEGER idx);
bool cgraph_2wheap_has_active(const cgraph_2wheap_t *h, CGRAPH_INTEGER idx);
CGRAPH_REAL cgraph_2wheap_get(const cgraph_2wheap_t *h, CGRAPH_INTEGER idx);
CGRAPH_REAL cgraph_2wheap_delete_max(cgraph_2wheap_t *h);
CGRAPH_REAL cgraph_2wheap_delete_max_index(cgraph_2wheap_t *h, CGRAPH_INTEGER *idx);
int cgraph_2wheap_modify(cgraph_2wheap_t *h, CGRAPH_INTEGER idx, CGRAPH_REAL elem);
int cgraph_2wheap_check(cgraph_2wheap_t *h);

#ifdef __cplusplus
}
#endif

#endif  // CGRAPH_TYPES_INTERNAL_H_