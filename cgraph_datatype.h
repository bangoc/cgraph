#ifndef CGRAPH_DATATYPE_H_
#define CGRAPH_DATATYPE_H_

#include "cgraph_basic_types.h"
#include "cgraph_ivec.h"

/**
 * \brief Cấu trúc dữ liệu biểu diễn đồ thị
 * \tham khảo: igraph_t
 *
 * Cấu trúc chỉ mục 2 tầng cho biểu diễn đồ thị dạng danh sách cạnh.
 * Nó có các thành phần sau:
 * - <b>n</b> Số lượng đỉnh, các đỉnh được quy ước là số nguyên không
 * âm có giá trị từ 0 đến n-1.
 * - <b>directed</b> Đồ thị có hướng (directed == true) hoặc vô
 * hướng (directed == false).
 * - <b>from</b> Cột đầu tiên của danh sách cạnh.
 * - <b>to</b> Cột thứ hai của danh sách cạnh.
 * - <b>oi</b> Chỉ số của cạnh được sắp xếp tăng dần theo cột thứ
 * nhất rồi đến cột thứ hai. Cạnh đầu tiên theo thứ tự này là
 * from[oi[0]], to[oi[0]], trong đó from[oi[0]] là nhỏ nhất trong tất * cả các giá trị from (sắp xếp chỉ số). Độ dài vec-tơ này bằng số
 * lượng cạnh có trong đồ thị.
 * - <b>ii</b> Chỉ số của cạnh được sắp xếp tăng dần theo cột thứ hai
 * rồi đến cột thứ nhất. Cạnh đầu tiên theo thứ tự này là
 * from[ii[0]] , to[ii[0]], trong đó to[ii[0]] có giá trị nhỏ nhất
 * trong tất cả các giá trị ii (sắp xếp chỉ số). Độ dài vec-tơ này
 * bằng số lượng cạnh có trong đồ thị.
 * - <b>os</b> Chứa các con trỏ tới danh sách cạnh đi ra từ tất cả
 * các đỉnh. Cạnh đầu tiên \em từ đỉnh v là cạnh \c from[oi[os[v]]],
 * to [oi[os[v]]] nếu \c os[v] < os[v+1]. Nếu \c os[v] == os[v+1] thì
 * không có cạnh đi ra từ đỉnh v. Độ dài của vec-tơ os bằng số lượng
 * đỉnh cộng 1, phần tử cuối cùng luôn bằng số lượng cạnh và được sử
 * dụng để tra cứu các cạnh cho đỉnh n-1.
 * - <b>is</b> Về cơ bản tương tự như <b>os</b>, nhưng đối với
 * các cạnh đi vào (vec-tơ ii).
 *
 * Đối với đồ thị vô hướng, cùng một danh sách cạnh được lưu, nghĩa
 * là một cạnh vô hướng chỉ được lưu một lần. Để kiểm tra có một cạnh
 * vô hướng từ \c v1 tới \c v2 hay không? - Chúng ta phải kiểm tra cả
 * hai trường hợp \c from=v1, \c to=v2 và \c from=v2, \c to=v1.
 *
 * Yêu cầu lưu trữ đồ thị với \c |V| đỉnh và \c |E| cạnh là
 * \c O(|E| + |V|)
 */

typedef struct cgraph_s {
    CGRAPH_INTEGER n;
    bool directed;
    cgraph_ivec_t from;
    cgraph_ivec_t to;
    cgraph_ivec_t oi;
    cgraph_ivec_t ii;
    cgraph_ivec_t os;
    cgraph_ivec_t is;
    void *attr;
} *cgraph_t;

#endif  // CGRAPH_DATATYPE_H_
