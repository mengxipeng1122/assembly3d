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

option(A3D_USE_GL_VAO "Use OpenGL vertex array." NO)
option(A3D_USE_FREEIMAGE "Use FreeImage for loading texture images." YES)

find_package(GLEW REQUIRED)

find_package(LibXml2 REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED libglfw)

if(GLFW_FOUND)
    message(STATUS "Found GLFW")
endif(GLFW_FOUND)

find_package(FreeImage)
if(FREEIMAGE_FOUND AND A3D_USE_FREEIMAGE)
    message(STATUS "Found FreeImage")
    message(STATUS "Using FreeImage for image loading")
    add_definitions("-DA3D_USE_FREEIMAGE")
else(FREEIMAGE_FOUND AND A3D_USE_FREEIMAGE)
    message(STATUS "FreeImage not found/using")
    message(STATUS "Using glfw for image loading")
endif(FREEIMAGE_FOUND AND A3D_USE_FREEIMAGE)

set(GLM_PACKAGE_DIR ${CMAKE_SOURCE_DIR}/external_libs/glm-0.9.2.3)
find_package(GLM REQUIRED)

#set(GLI_PACKAGE_DIR ${CMAKE_SOURCE_DIR}/external_libs/gli-0.3.0.3)
#find_package(GLI REQUIRED)

set(TCLAP_INCLUDE ${CMAKE_SOURCE_DIR}/external_libs/tclap-1.2.1/include)

add_definitions(${GLFW_CFLAGS_OTHER})

if(A3D_USE_GL_VAO)
    add_definitions( "-DA3D_GL_USE_VAO")
endif(A3D_USE_GL_VAO)

include_directories(${GLFW_INCLUDE_DIRS}
                    ${GLEW_INCLUDE_PATH}
                    ${LIBXML2_INCLUDE_DIR}
                    ${TCLAP_INCLUDE}
                    ${FREEIMAGE_INCLUDE_DIRS})

link_directories(${GLFW_LIBRARY_DIRS})

IF(APPLE)
    SET(EXTRA_LIBS "-framework Cocoa -framework OpenGL")
ELSE(APPLE)
    SET(EXTRA_LIBS ${GLFW_LDFLAGS_OTHER})
ENDIF(APPLE)

set(A3DViewer_HEADER src/App.h
                     src/Graphics.h
                     src/Mesh.h
                     src/ShaderUtils.h
                     src/ProgramSimple.h
                     src/Resources.h
                     src/Location3D.h
                     src/MeshLoader.h
                     src/Utils.h
   )

set(A3DViewer_SOURCE src/Main.cpp
                     src/App.cpp
                     src/Graphics.cpp
                     src/Mesh.cpp
                     src/ShaderUtils.cpp
                     src/ProgramSimple.cpp
                     src/MeshLoader.cpp
                     src/Utils.cpp
   )

add_executable(A3DViewer
               ${A3DViewer_SOURCE}
               ${A3DViewer_HEADER}
              )

target_link_libraries(A3DViewer
                      ${GLFW_LIBRARIES}
                      ${EXTRA_LIBS}
                      ${GLEW_LIBRARY}
                      ${LIBXML2_LIBRARIES}
                      ${FREEIMAGE_LIBRARY}
                      )
