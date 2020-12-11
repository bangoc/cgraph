cmake_minimum_required(VERSION 2.8)
project(collectc)

set(source_files
  array.c
  common.c
  deque.c
  hashset.c
  hashtable.c
  list.c
  pqueue.c
  queue.c
  ring_buffer.c
  slist.c
  stack.c
  treeset.c
  treetable.c
  tsttable.c
  my/hsi.c)
set(header_files
  array.h
  common.h
  deque.h
  hashset.h
  hashtable.h
  list.h
  pqueue.h
  queue.h
  ring_buffer.h
  slist.h
  stack.h
  treeset.h
  treetable.h
  tsttable.h
)
set(my_header_files
  my/hsi.h
)

add_library(${PROJECT_NAME} STATIC ${source_files})

set(ctc_include_dest "ctc")
set(ctc_lib_dest "lib")
install(FILES ${header_files} DESTINATION ${ctc_include_dest})
install(FILES ${my_header_files} DESTINATION ${ctc_include_dest}/my)
install(TARGETS ${PROJECT_NAME} DESTINATION ${ctc_lib_dest})