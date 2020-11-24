cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0048 NEW)
project(collectc VERSION 0.0.1)

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
  tsttable.c)
set(header_files 
  include/array.h
  include/common.h
  include/deque.h
  include/hashset.h
  include/hashtable.h
  include/list.h
  include/pqueue.h 
  include/queue.h
  include/ring_buffer.h 
  include/slist.h
  include/stack.h
  include/treeset.h
  include/treetable.h 
  include/tsttable.h)

add_library(${PROJECT_NAME} STATIC ${source_files})
include_directories(./include)
