cmake_minimum_required(VERSION 2.8)
project(A3DViewer)

if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "Build type defaulting to \"Release\"")
  set(CMAKE_BUILD_TYPE "Release")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake_modules
                      ${CMAKE_MODULE_PATH})

if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_C_FLAGS_DEBUG "-g -Wall -Wextra -DDEBUG -D_DEBUG")
  set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra -DDEBUG -D_DEBUG")
endif(CMAKE_COMPILER_IS_GNUCXX)

if(MSVC)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif(MSVC)

####################

#set(A3DMeshLoader_HEADER 
#    lib/A3DMeshParser.h
#    lib/A3DMeshLoader.h
#    lib/A3DMesh.h)
#set(A3DMeshLoader_SOURCE 
#    lib/A3DMeshParser.cpp
#    lib/A3DMeshLoader.cpp
#    lib/A3DMesh.cpp)
#
#add_library(A3DMeshLoader ${A3DMeshLoader_HEADER} ${A3DMeshLoader_SOURCE})

####################

find_package(GLEW REQUIRED)

find_package(LibXml2 REQUIRED)
message(STATUS ${LIBXML2_LIBRARIES})
message(STATUS ${LIBXML2_INCLUDE_DIR})
message(STATUS ${LIBXML2_DEFINITIONS})

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED libglfw)

if(GLFW_FOUND)
    message(STATUS "Found GLFW")
endif(GLFW_FOUND)

add_definitions(${GLFW_CFLAGS_OTHER})

include_directories(${GLFW_INCLUDE_DIRS} ${GLEW_INCLUDE_PATH} ${LIBXML2_INCLUDE_DIR} lib)

link_directories(${GLFW_LIBRARY_DIRS})

IF(APPLE)
    SET(EXTRA_LIBS "-framework Cocoa -framework OpenGL")
ELSE(APPLE)
    SET(EXTRA_LIBS ${GLFW_LDFLAGS_OTHER})
ENDIF (APPLE)

#set(A3DViewer_HEADER ) 

set(A3DViewer_SOURCE src/Main.cpp )

add_executable(A3DViewer ${A3DViewer_SOURCE})# ${A3DViewer_HEADER})

target_link_libraries(A3DViewer ${GLFW_LIBRARIES} ${EXTRA_LIBS} ${GLEW_LIBRARY} ${LIBXML2_LIBRARIES})

execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory
                        ${CMAKE_SOURCE_DIR}/glsl
                        ${CMAKE_BINARY_DIR}/glsl)
