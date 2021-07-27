#include "cgraph_interface.h"
#include "cgraph_foreign.h"
#include "cgen/gtvector.h"
/**
 * \function cgraph_write_graph_dot
 * \brief Xuất đồ thị ra outstream theo định dạng dot
 *
 * DOT là định dạng được sử dụng bởi phần mềm GraphViz, tham khảo
 * http://www.graphviz.org, và tài liệu về định dạng DOT
 * http://www.graphviz.org/doc/info/lang.html
 *
 * </para><para>Đây là một triển khai sơ lược, chỉ các đỉnh và các cạnh
 * được xuất, các thuộc tính và bất kỳ thông tin trực quan nào khác
 * không được xuất.
 *
 * \param graph Đồ thị cần được xuất ra tệp.
 * \param outstream Luồng gắn với tệp.
 *
 * Độ phức tạp thời gian: Tỉ lệ thuận với số lượng ký tự cần xuất.
 *
 * \sa \ref igraph_write_graph_graphml() for định dạng dựa trên xml.
 *
 * \example examples/foreign/dot.c
 */
int cgraph_write_graph_dot(const cgraph_t graph, FILE* outstream) {
    long int i;
    long int no_of_nodes = cgraph_vcount(graph);
    long int no_of_edges = cgraph_ecount(graph);
    char edgeop[3];

    // TODO: Bổ xung thông tin phiên bản
    fprintf(outstream, "/* Được tạo bởi cgraph */\n");

    if (cgraph_is_directed(graph)) {
        fprintf(outstream, "digraph {\n");
        strcpy(edgeop, "->");
    } else {
        fprintf(outstream, "graph {\n");
        strcpy(edgeop, "--");
    }

    // TODO: Xuất các thuộc tính

    /* Xuất các đỉnh */
    for (i = 0; i < no_of_nodes; i++) {
      fprintf(outstream, "  %ld;\n", i);
    }
    fprintf(outstream, "\n");

    /* Xuất các cạnh */
    for (i = 0; i < no_of_edges; i++) {
        long int from = CGRAPH_FROM(graph, i);
        long int to = CGRAPH_TO(graph, i);
        fprintf(outstream, "  %ld %s %ld;\n", from, edgeop, to);
    }
    fprintf(outstream, "}\n");

    return 0;
}