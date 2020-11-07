project (cgraph C)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_C_STANDARD 99)

include_directories(${CMAKE_SOURCE_DIR})
add_subdirectory(examples)

if (${CMAKE_PROJECT_NAME} STREQUAL cgraph)
  include(CTest)
endif()

if(${BUILD_TESTING})  
  add_subdirectory(tests)
  add_test(NAME CvectorCreateEmpty COMMAND cvector_create_empty_ut)
endif()