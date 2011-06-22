project(Assembly3DTools)

cmake_minimum_required(VERSION 2.6)

if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "No build type selected, default to Release")
  set(CMAKE_BUILD_TYPE "Release")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules
                      ${CMAKE_MODULE_PATH})

configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

add_custom_target(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")


if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ggdb -Wall -Wextra -Wconversion -DDEBUG -D_DEBUG")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb -Wall -Wextra -Wconversion -DDEBUG -D_DEBUG")
endif(CMAKE_COMPILER_IS_GNUCXX)

if(MSVC)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif(MSVC)

add_subdirectory(common)
add_subdirectory(mesh_wiz)
add_subdirectory(mesh_test)
add_subdirectory(mesh_prim)
