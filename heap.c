#include "cgraph_types_internal.h"

#include "cgraph_error.h"

/* -------------------------------------------------- */
/* Cấu trúc heap được đánh chỉ mục 2 chiều             */
/* -------------------------------------------------- */

#define PARENT(x)   (((x)+1)/2-1)
#define LEFTCHILD(x)  (((x)+1)*2-1)
#define RIGHTCHILD(x) (((x)+1)*2)

/* Đây là một cấu trúc heap được đánh chỉ mục 2 chiều. Ngoài các đặc
   tính của heap được đánh chỉ mục thông thường. Nó cho phép truy cập
   tới tất cả các phần tử của nó trong thời gian O(1). Nói cách khác,
   thao tác cập nhật giá trị cho cấu trúc heap này có độ phức tạp là
   O(1), còn đối với heap thường thao tác cập nhật được thực hiện
   với độ phức tạp O(n).... */

void cgraph_i_2wheap_switch(cgraph_2wheap_t *h,
              CGRAPH_INTEGER e1, CGRAPH_INTEGER e2) {
  if (e1 != e2) {
    CGRAPH_INTEGER tmp1, tmp2;
    CGRAPH_REAL tmp3 = h->data[e1];
    h->data[e1] = h->data[e2];
    h->data[e2] = tmp3;

    tmp1 = h->index[e1];
    tmp2 = h->index[e2];

    h->index2[tmp1] = e2 + 2;
    h->index2[tmp2] = e1 + 2;

    h->index[e1] = tmp2;
    h->index[e2] = tmp1;
  }
}

void cgraph_i_2wheap_shift_up(cgraph_2wheap_t *h,
                CGRAPH_INTEGER elem) {
  if (elem == 0 || h->data[elem] < h->data[PARENT(elem)]) {
    /* Đỉnh của heap */
  } else {
    cgraph_i_2wheap_switch(h, elem, PARENT(elem));
    cgraph_i_2wheap_shift_up(h, PARENT(elem));
  }
}

void cgraph_i_2wheap_sink(cgraph_2wheap_t *h,
              CGRAPH_INTEGER head) {
  CGRAPH_INTEGER size = cgraph_2wheap_size(h);
  if (LEFTCHILD(head) >= size) {
    /* Không có cây con */
  } else if (RIGHTCHILD(head) == size ||
         h->data[LEFTCHILD(head)] >= h->data[RIGHTCHILD(head)]) {
    /* Nhúng vào nhánh trái nếu cần */
    if (h->data[head] < h->data[LEFTCHILD(head)]) {
      cgraph_i_2wheap_switch(h, head, LEFTCHILD(head));
      cgraph_i_2wheap_sink(h, LEFTCHILD(head));
    }
  } else {
    /* Nhúng vào nhánh phải */
    if (h->data[head] < h->data[RIGHTCHILD(head)]) {
      cgraph_i_2wheap_switch(h, head, RIGHTCHILD(head));
      cgraph_i_2wheap_sink(h, RIGHTCHILD(head));
    }
  }
}

/* ------------------ */
/* Các hàm công khai   */
/* ------------------ */

int cgraph_2wheap_init(cgraph_2wheap_t *h, CGRAPH_INTEGER size) {
  h->size = size;
  h->data = cgraph_rvec_create();
  h->index = cgraph_ivec_create();
  h->index2 = cgraph_ivec_create();

  /* Chúng ta bắt đầu với giá trị lớn nhất */
  CGRAPH_CHECK(cgraph_ivec_init(&h->index2, size));
  return 0;
}

void cgraph_2wheap_free(cgraph_2wheap_t *h) {
  cgraph_rvec_free(&h->data);
  cgraph_ivec_free(&h->index);
  cgraph_ivec_free(&h->index2);
}

int cgraph_2wheap_clear(cgraph_2wheap_t *h) {
  cgraph_rvec_setsize(h->data, 0);
  cgraph_ivec_setsize(h->index, 0);
  cgraph_ivec_setsize(h->index2, 0);
  return 0;
}

bool cgraph_2wheap_empty(const cgraph_2wheap_t *h) {
  return cgraph_rvec_size(h->data) == 0;
}

int cgraph_2wheap_push_with_index(cgraph_2wheap_t *h,
                  CGRAPH_INTEGER idx, CGRAPH_REAL elem) {

  /*   printf("-> %.2g [%li]\n", elem, idx); */
  CGRAPH_INTEGER size = cgraph_rvec_size(h->data);
  CGRAPH_CHECK(cgraph_rvec_push_back(&h->data, elem));
  CGRAPH_CHECK(cgraph_ivec_push_back(&h->index, idx));
  h->index2[idx] = size + 2;

  /* Duy trì heap */
  cgraph_i_2wheap_shift_up(h, size);
  return 0;
}

CGRAPH_INTEGER cgraph_2wheap_size(const cgraph_2wheap_t *h) {
  return cgraph_rvec_size(h->data);
}

CGRAPH_INTEGER cgraph_2wheap_max_size(const cgraph_2wheap_t *h) {
  return h->size;
}

CGRAPH_REAL cgraph_2wheap_max(const cgraph_2wheap_t *h) {
  return h->data[0];
}

CGRAPH_INTEGER cgraph_2wheap_max_index(const cgraph_2wheap_t *h) {
  return h->index[0];
}

bool cgraph_2wheap_has_elem(const cgraph_2wheap_t *h, CGRAPH_INTEGER idx) {
  return h->index2[idx] != 0;
}

bool cgraph_2wheap_has_active(const cgraph_2wheap_t *h, CGRAPH_INTEGER idx) {
  return h->index2[idx] > 1;
}

CGRAPH_REAL cgraph_2wheap_get(const cgraph_2wheap_t *h, CGRAPH_INTEGER idx) {
  CGRAPH_INTEGER i = h->index2[idx] - 2;
  return h->data[i];
}

CGRAPH_REAL cgraph_2wheap_delete_max(cgraph_2wheap_t *h) {
  CGRAPH_REAL tmp = h->data[0];
  CGRAPH_INTEGER tmpidx = h->index[0];
  cgraph_i_2wheap_switch(h, 0, cgraph_2wheap_size(h) - 1);
  cgraph_rvec_setsize(h->data, cgraph_rvec_size(h->data) - 1);
  cgraph_ivec_setsize(h->index, cgraph_ivec_size(h->index) - 1);
  h->index2[tmpidx] = 0;
  cgraph_i_2wheap_sink(h, 0);

  /*   printf("<-max %.2g\n", tmp); */
  return tmp;
}

CGRAPH_REAL cgraph_2wheap_deactivate_max(cgraph_2wheap_t *h) {

  CGRAPH_REAL tmp = h->data[0];
  CGRAPH_INTEGER tmpidx = h->index[0];
  cgraph_i_2wheap_switch(h, 0, cgraph_2wheap_size(h) - 1);
  cgraph_rvec_setsize(h->data, cgraph_rvec_size(h->data) - 1);
  cgraph_ivec_setsize(h->index, cgraph_ivec_size(h->index) - 1);
  h->index2[tmpidx] = 1;
  cgraph_i_2wheap_sink(h, 0);

  return tmp;
}

CGRAPH_REAL cgraph_2wheap_delete_max_index(cgraph_2wheap_t *h, CGRAPH_INTEGER *idx) {

  CGRAPH_REAL tmp = h->data[0];
  CGRAPH_INTEGER tmpidx = h->index[0];
  cgraph_i_2wheap_switch(h, 0, cgraph_2wheap_size(h) - 1);
  cgraph_rvec_setsize(h->data, cgraph_rvec_size(h->data) - 1);
  cgraph_ivec_setsize(h->index, cgraph_ivec_size(h->index) - 1);
  h->index2[tmpidx] = 0;
  cgraph_i_2wheap_sink(h, 0);

  if (idx) {
    *idx = tmpidx;
  }
  return tmp;
}

int cgraph_2wheap_modify(cgraph_2wheap_t *h, CGRAPH_INTEGER idx, CGRAPH_REAL elem) {

  CGRAPH_INTEGER pos = h->index2[idx] - 2;

  /*   printf("-- %.2g -> %.2g\n", VECTOR(h->data)[pos], elem); */

  h->data[pos] = elem;
  cgraph_i_2wheap_sink(h, pos);
  cgraph_i_2wheap_shift_up(h, pos);

  return 0;
}

/* Kiểm tra liệu heap có ở trong trạng thái hợp lệ không */

int cgraph_2wheap_check(cgraph_2wheap_t *h) {
  CGRAPH_INTEGER size = cgraph_2wheap_size(h);
  CGRAPH_INTEGER i;
  bool error = 0;

  /* Kiểm tra tính chất heap */
  for (i = 0; i < size; i++) {
    if (LEFTCHILD(i) >= size) {
      break;
    }
    if (h->data[LEFTCHILD(i)] > h->data[i]) {
      error = 1; break;
    }
    if (RIGHTCHILD(i) >= size) {
      break;
    }
    if (h->data[RIGHTCHILD(i)] > h->data[i]) {
      error = 1;
      break;
    }
  }

  if (error) {
    CGRAPH_ERROR("Heap không nhất quán", CGRAPH_FAILURE);
    return 1;
  }

  return 0;
}
