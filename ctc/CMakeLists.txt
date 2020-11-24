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
  tsttable.h)

add_library(${PROJECT_NAME} STATIC ${source_files})
include_directories(./include)
