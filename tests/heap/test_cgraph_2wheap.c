#include "cgraph.h"
#include "ut.h"

#include <stdio.h>
#include <stdlib.h>

int main() {

  cgraph_rvec_t elems = cgraph_rvec_create();
  cgraph_2wheap_t Q;
  CGRAPH_INTEGER i;
  CGRAPH_REAL CGRAPH_INFINITY = 1.0/0.0;
  CGRAPH_REAL prev = CGRAPH_INFINITY;

  srand(42); /* make tests deterministic */

  cgraph_rvec_init(&elems, 100);
  for (i = 0; i < cgraph_rvec_size(elems); i++) {
    elems[i] = rand() / (double)RAND_MAX;
  }

  cgraph_2wheap_init(&Q, cgraph_rvec_size(elems));
  for (i = 0; i < cgraph_rvec_size(elems); i++) {
    cgraph_2wheap_push_with_index(&Q, i, elems[i]);
  }

  /*****/

  for (i = 0; i < cgraph_rvec_size(elems); i++) {
    if (elems[i] != cgraph_2wheap_get(&Q, i)) {
      UT_MSG_FAILED("Case 1. Size check");
      return 1;
    }
  }

  /*****/

  for (i = 0; i < cgraph_rvec_size(elems); i++) {
    CGRAPH_INTEGER j;
    CGRAPH_REAL tmp = cgraph_2wheap_max(&Q);
    if (tmp > prev) {
      UT_MSG_FAILED("Case 2. 2wheap max");
      return 2;
    }
    if (tmp != cgraph_2wheap_delete_max_index(&Q, &j)) {
      UT_MSG_FAILED("Case 3. Delete 2wheap max");
      return 3;
    }
    if (elems[j] != tmp) {
      UT_MSG_FAILED("Case 4. Check original value");
      return 4;
    }
    prev = tmp;
  }

  /*****/

  for (i = 0; i < cgraph_rvec_size(elems); i++) {
    cgraph_2wheap_push_with_index(&Q, i, elems[i]);
  }
  if (cgraph_2wheap_size(&Q) != cgraph_rvec_size(elems)) {
    UT_MSG_FAILED("Case 5. Size check");
    return 5;
  }
  for (i = 0; i < cgraph_rvec_size(elems); i++) {
    elems[i] = rand() / (double)RAND_MAX;
    cgraph_2wheap_modify(&Q, i, elems[i]);
  }
  for (i = 0; i < cgraph_rvec_size(elems); i++) {
    if (elems[i] != cgraph_2wheap_get(&Q, i)) {
      UT_MSG_FAILED("Case 6. Get i elem");
      return 6;
    }
  }
  prev = CGRAPH_INFINITY;
  for (i = 0; i < cgraph_rvec_size(elems); i++) {
    CGRAPH_INTEGER j;
    CGRAPH_REAL tmp = cgraph_2wheap_max(&Q);
    if (tmp > prev) {
      UT_MSG_FAILED("Case 7. 2wheap max");
      return 7;
    }
    if (tmp != cgraph_2wheap_delete_max_index(&Q, &j)) {
      UT_MSG_FAILED("Case 8. Delete max index");
      return 8;
    }
    if (elems[j] != tmp) {
      UT_MSG_FAILED("Case 9. Original element");
      return 9;
    }
    prev = tmp;
  }
  if (!cgraph_2wheap_empty(&Q)) {
    UT_MSG_FAILED("Case 10. Empty");
    return 10;
  }
  if (cgraph_2wheap_size(&Q) != 0) {
    UT_MSG_FAILED("Case 11. 2wheap zero size");
    return 11;
  }

  cgraph_2wheap_free(&Q);
  cgraph_rvec_free(&elems);

  /* Hand-made example */

#define MAX     do { \
  cgraph_2wheap_delete_max(&Q); \
  if (cgraph_2wheap_check(&Q)) { \
    UT_MSG_FAILED("Case 12. Hand check, delete max"); \
    return 1; \
  } \
} while (0)
#define PUSH(i,e) do { \
  cgraph_2wheap_push_with_index(&Q, (i), -(e)); \
  if (cgraph_2wheap_check(&Q)) { \
    UT_MSG_FAILED("Case 13. Hand check, push with index"); \
    return 1; \
  } \
} while (0);
#define MOD(i, e) do { \
  cgraph_2wheap_modify(&Q, (i), -(e));\
  if (cgraph_2wheap_check(&Q)) { \
    UT_MSG_FAILED("Case 14. Hand check, modify"); \
    return 1; \
  } \
} while (0)

  cgraph_2wheap_init(&Q, 21);
  /* 0.00 [ 4] */ PUSH(4, 0);
  /* MAX     */ MAX;
  /* 0.63 [11] */ PUSH(11, 0.63);
  /* 0.05 [15] */ PUSH(15, 0.05);
  /* MAX     */ MAX;
  /* 0.4  [12] */ PUSH(12, 0.4);
  /* 0.4  [13] */ PUSH(13, 0.4);
  /* 0.12 [16] */ PUSH(16, 0.12);
  /* MAX     */ MAX;
  /* 1.1  [ 0] */ PUSH(0, 1.1);
  /* 1.1  [14] */ PUSH(14, 1.1);
  /* MAX     */ MAX;
  /* [11]/0.44 */ MOD(11, 0.44);
  /* MAX     */ MAX;
  /* MAX     */ MAX;
  /* 1.1  [20] */ PUSH(20, 1.1);
  /* MAX     */ MAX;
  /* 1.3  [ 7] */ PUSH(7, 1.3);
  /* 1.7  [ 9] */ PUSH(9, 1.7);
  /* MAX     */ MAX;
  /* 1.6  [19] */ PUSH(19, 1.6);
  /* MAX     */ MAX;
  /* 2.1  [17] */ PUSH(17, 2.1);
  /* 1.3  [18] */ PUSH(18, 1.3);
  /* MAX     */ MAX;
  /* 2.3  [ 1] */ PUSH(1, 2.3);
  /* 2.2  [ 5] */ PUSH(5, 2.2);
  /* 2.3  [10] */ PUSH(10, 2.3);
  /* MAX     */ MAX;
  /* [17]/1.5  */ MOD(17, 1.5);
  /* MAX     */ MAX;
  /* 1.8  [ 6] */ PUSH(6, 1.8);
  /* MAX     */ MAX;
  /* 1.3  [ 3] */ PUSH(3, 1.3);
  /* [ 6]/1.3  */ MOD(6, 1.3);
  /* MAX     */ MAX;
  /* 1.6  [ 8] */ PUSH(8, 1.6);
  /* MAX     */ MAX;

  cgraph_2wheap_free(&Q);
  UT_MSG_OK("Cgraph 2wheap test");
  return 0;
}
