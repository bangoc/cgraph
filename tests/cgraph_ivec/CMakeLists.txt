add_executable(cgraph_ivec_max_ut
    test_cgraph_ivec_max.c)
add_executable(cgraph_ivec_isininterval_ut
    test_cgraph_ivec_isininterval.c)
add_executable(cgraph_ivec_grow_and_setsize_ut
    test_cgraph_ivec_grow_and_setsize.c)
add_executable(cgraph_ivec_order_ut
    test_cgraph_ivec_order.c)
add_executable(cgraph_ivec_as_value_ut
    test_cgraph_ivec_as_value.c)
add_executable(cgraph_ivec_as_ref_ut
    test_cgraph_ivec_as_ref.c)
add_executable(cgraph_ivec_append_ut
    test_cgraph_ivec_append.c)
add_executable(cgraph_ivec_from_array_ut
    test_cgraph_ivec_from_array.c)
foreach(t cgraph_ivec_max_ut
        cgraph_ivec_grow_and_setsize_ut
        cgraph_ivec_isininterval_ut
        cgraph_ivec_order_ut
        cgraph_ivec_as_value_ut
        cgraph_ivec_as_ref_ut
        cgraph_ivec_append_ut
        cgraph_ivec_from_array_ut)
    target_link_libraries(${t} ut_base cgraph)
endforeach()