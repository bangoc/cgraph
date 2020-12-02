add_executable(cgraph_ivec_max_ut 
    test_cgraph_ivec_max.c 
    ${CMAKE_SOURCE_DIR}/cgraph_ivec.c)
add_executable(cgraph_ivec_isininterval_ut 
    test_cgraph_ivec_isininterval.c 
    ${CMAKE_SOURCE_DIR}/cgraph_ivec.c)
add_executable(cgraph_ivec_grow_and_setsize_ut 
    test_cgraph_ivec_grow_and_setsize.c 
    ${CMAKE_SOURCE_DIR}/cgraph_ivec.c)
add_executable(cgraph_ivec_order_ut 
    test_cgraph_ivec_order.c 
    ${CMAKE_SOURCE_DIR}/cgraph_ivec.c)
add_executable(cgraph_ivec_as_value_ut 
    test_cgraph_ivec_as_value.c
    ${CMAKE_SOURCE_DIR}/cgraph_ivec.c)
add_executable(cgraph_ivec_as_ref_ut 
    test_cgraph_ivec_as_ref.c
    ${CMAKE_SOURCE_DIR}/cgraph_ivec.c)

target_link_libraries(cgraph_ivec_grow_and_setsize_ut cgraph)
target_link_libraries(cgraph_ivec_order_ut ut_base cgraph)
target_link_libraries(cgraph_ivec_as_value_ut ut_base)
target_link_libraries(cgraph_ivec_as_ref_ut ut_base)