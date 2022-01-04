add_executable(pq_demo pqueue_demo.c)
foreach(app pq_demo)
  target_link_libraries(${app} collectc cgraph)
endforeach()
