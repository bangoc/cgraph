#ifndef CGRAPH_INTERFACE_H_
#define CGRAPH_INTERFACE_H_

#include "cgen/vec/arr.h"
#include "base/cgraph_datatype.h"
#include "base/cgraph_constants.h"

#ifdef __cplusplus
extern "C"{
#endif

void cgraph_add_vertices(cgraph_t graph, CGRAPH_INTEGER nv);
void cgraph_delete_edges(cgraph_t graph, atype(CGRAPH_INTEGER) *edges);
void cgraph_disconnect_vertices(cgraph_t graph,
      atype(CGRAPH_INTEGER) *vertices, cgraph_neimode_t mode);
void cgraph_add_edges(cgraph_t graph, atype(CGRAPH_INTEGER) *edges);
CGRAPH_INTEGER cgraph_vcount(const cgraph_t graph);
CGRAPH_INTEGER cgraph_ecount(const cgraph_t graph);
bool cgraph_is_directed(const cgraph_t graph);
atype(CGRAPH_INTEGER) *cgraph_neighbors(const cgraph_t graph,
                     CGRAPH_INTEGER vid,
                     cgraph_neimode_t mode);
atype(CGRAPH_INTEGER) *cgraph_incident(const cgraph_t graph,
                    CGRAPH_INTEGER vid, cgraph_neimode_t mode);

atype(CGRAPH_INTEGER) *cgraph_degree_all(const cgraph_t graph,
                      cgraph_neimode_t mode, bool loops);

CGRAPH_INTEGER cgraph_degree_one(const cgraph_t graph,
                      const CGRAPH_INTEGER node,
                      cgraph_neimode_t mode,
                      bool loops);
struct edge cgraph_edge(const cgraph_t graph, CGRAPH_INTEGER eid);
CGRAPH_INTEGER cgraph_get_eid(const cgraph_t graph,
                   CGRAPH_INTEGER pfrom, CGRAPH_INTEGER pto, bool directed);

#ifdef __cplusplus
}
#endif

/**
 * \define CGRAPH_FROM
 * \brief Đỉnh nguồn của một cạnh
 *
 * Nhanh hơn cgraph_edge nhưng không kiểm tra lỗi và giả sử eid
 * hợp lệ
 *
 * \param graph Đối tượng đồ thị.
 * \param eid   Chỉ số cạnh.
 * \return Trả về đỉnh nguồn của cạnh.
 * \sa \ref cgraph_edge() Nếu muốn có kiểm tra lỗi.
 */
#define CGRAPH_FROM(graph,eid) ((CGRAPH_INTEGER)(graph->from[(long int)(eid)]))

/**
 * \define CGRAPH_TO
 * \brief Đỉnh đích của một cạnh.
 *
 * Nhanh hơn \ref cgraph_edge nhưng không kiểm tra lỗi và giả sử eid
 * hợp lệ.
 *
 * \param graph Đối tượng đồ thị.
 * \param eid   Chỉ số của cạnh.
 * \return Trả về đỉnh đích của cạnh.
 * \sa \ref cgraph_edge() Nếu muốn có kiểm tra lỗi.
 */
#define CGRAPH_TO(graph,eid)   ((CGRAPH_INTEGER)(graph->to[(long int)(eid)]))

/**
 * \define CGRAPH_OTHER
 * \brief Đỉnh phía bên kia của một cạnh.
 *
 * Thường được sử dụng với các cạnh vô hướng khi đã biết một đỉnh
 * của cạnh, và cần đỉnh ở phía bên kia. Không kiểm tra lỗi, eid và
 * vid được cho là hợp lệ.
 *
 * \param graph Đối tượng đồ thị.
 * \param eid   Chỉ số cạnh.
 * \param vid   Một đỉnh của cạnh.
 * \return Trả về đỉnh phía bên kia của cạnh.
 * \sa \ref CGRAPH_TO() và \ref CGRAPH_FROM() để lấy nguồn và đích
 * của cạnh có hướng.
 */
#define CGRAPH_OTHER(graph,eid,vid) \
    ((CGRAPH_INTEGER)(CGRAPH_TO(graph,(eid))==(vid) ? CGRAPH_FROM((graph),(eid)) : CGRAPH_TO((graph),(eid))))

/**
 * \ingroup interface
 * \function cgraph_destroy
 * \brief Giải phóng bộ nhớ đã được cấp phát cho một đồ thị.
 *
 * </para><para>
 * Hàm này chỉ được gọi đúng một lần cho một đồ thị.
 *
 * </para><para>
 * Bộ nhớ được cấp phát cho biểu diễn đồ thị phải được hủy trước khi
 * hủy bộ nhớ cho chính cấu trúc đồ thị.
 *
 * @param g Con trỏ tới đồ thị cần giải phóng.
 * @return Không trả về giá trị
 *
 * Độ phức tạp: phụ thuộc vào hệ điều hành.
 */
#define cgraph_destroy(g) \
  do { \
    arr_free(g->from); \
    arr_free(g->to); \
    arr_free(g->oi); \
    arr_free(g->ii); \
    arr_free(g->os); \
    arr_free(g->is); \
    free(g); \
  } while (0)

#endif  // CGRAPH_INTERFACE_H_
