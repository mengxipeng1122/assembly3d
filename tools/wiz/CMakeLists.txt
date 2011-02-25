project(Assembly3DWiz)

cmake_minimum_required(VERSION 2.6)

if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, default to Release")
  set(CMAKE_BUILD_TYPE "Release")
endif()

if(UNIX)
  set(CMAKE_C_FLAGS -m32)
  set(CMAKE_CXX_FLAGS -m32)
  set(CMAKE_C_FLAGS_DEBUG "-g -Wall -DDEBUG -D_DEBUG")
  set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -DDEBUG -D_DEBUG -pedantic")
endif(UNIX)

add_definitions(-DTIXML_USE_STL)

add_subdirectory(external_libs)
add_subdirectory(common)
add_subdirectory(mesh_wiz)
