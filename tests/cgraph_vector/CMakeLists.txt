add_executable(cgraph_ivec_max_ut 
    test_cgraph_ivec_max.c 
    ${CMAKE_SOURCE_DIR}/cgraph_vector.c)
add_executable(cgraph_ivec_isininterval_ut 
    test_cgraph_ivec_isininterval.c 
    ${CMAKE_SOURCE_DIR}/cgraph_vector.c)
add_executable(cgraph_ivec_grow_and_setsize_ut 
    test_cgraph_ivec_grow_and_setsize.c 
    ${CMAKE_SOURCE_DIR}/cgraph_vector.c)
add_executable(cgraph_ivec_order_ut 
    test_cgraph_ivec_order.c 
    ${CMAKE_SOURCE_DIR}/cgraph_vector.c)

target_link_libraries(cgraph_ivec_order_ut ut_base)