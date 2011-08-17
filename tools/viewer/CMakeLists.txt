cmake_minimum_required(VERSION 2.8)
project(A3DViewer)

if (NOT CMAKE_BUILD_TYPE)
  message(STATUS "Build type defaulting to \"Release\"")
  set(CMAKE_BUILD_TYPE "Release")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake_scripts
                      ${CMAKE_MODULE_PATH})

if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_C_FLAGS_DEBUG "-g -Wall -Wextra -DDEBUG -D_DEBUG")
  set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wextra -DDEBUG -D_DEBUG")
endif(CMAKE_COMPILER_IS_GNUCXX)

if(MSVC)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_CONSOLE)
endif(MSVC)

####################

option(A3D_USE_GL_VAO "Use OpenGL vertex array." NO)
option(A3D_USE_FREEIMAGE "Use FreeImage for loading texture images." YES)

if(MSVC)
	set(GLEW_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external_libs/glew-1.6.0)
	set(GLFW_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external_libs/glfw-2.7.1.bin.WIN32)
	set(FREEIMAGE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external_libs/FreeImage/Dist)
	set(LIBXML2_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external_libs/libxml2-2.7.8.win32)
	set(ICONV_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external_libs/iconv-1.9.2.win32)
	set(ZLIB_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external_libs/zlib-1.2.5)
	
	set(LIBXML2_INCLUDE_DIR ${LIBXML2_ROOT_DIR}/include/ ${ICONV_ROOT_DIR}/include ${ZLIB_ROOT_DIR}/include)
	set(LIBXML2_LIBRARIES ${LIBXML2_ROOT_DIR}/lib/libxml2.lib ${ICONV_ROOT_DIR}/lib/iconv_a.lib ${ZLIB_ROOT_DIR}/lib/zlib.lib)
	set(LIBXML2_XMLLINT_EXECUTABLE ${LIBXML2_ROOT_DIR}/bin)
	find_package(GLFW REQUIRED)
else(MSVC)
	find_package(LibXml2 REQUIRED)
	find_package(PkgConfig REQUIRED)
	pkg_search_module(GLFW REQUIRED libglfw)
endif(MSVC)

if(GLFW_FOUND)
    message(STATUS "Found GLFW")
endif(GLFW_FOUND)

find_package(GLEW REQUIRED)

find_package(FreeImage)
#set(FREEIMAGE_FOUND 0)
if(FREEIMAGE_FOUND AND A3D_USE_FREEIMAGE)
    message(STATUS "Found FreeImage")
    message(STATUS "Using FreeImage for image loading")
    set(A3D_FREEIMAGE True)
    set(FREEIMAGE_INCLUDES ${FREEIMAGE_INCLUDE_DIRS})
    set(FREEIMAGE_LIBS ${FREEIMAGE_LIBRARY})
else(FREEIMAGE_FOUND AND A3D_USE_FREEIMAGE)
    message(STATUS "FreeImage not found/using")
    message(STATUS "Using glfw for image loading")
    set(A3D_FREEIMAGE False)
endif(FREEIMAGE_FOUND AND A3D_USE_FREEIMAGE)

set(GLM_PACKAGE_DIR ${CMAKE_SOURCE_DIR}/external_libs/glm-0.9.2.3)
find_package(GLM REQUIRED)

if(A3D_USE_GL_VAO)
    message(STATUS "Using vertex array object")
    set(A3D_GL_VAO True)
else(A3D_USE_GL_VAO)
    set(A3D_GL_VAO False)
endif(A3D_USE_GL_VAO)

configure_file(${CMAKE_SOURCE_DIR}/cmake_scripts/config.h.in ${CMAKE_SOURCE_DIR}/src/config.h)

#set(GLI_PACKAGE_DIR ${CMAKE_SOURCE_DIR}/external_libs/gli-0.3.0.3)
#find_package(GLI REQUIRED)

set(TCLAP_INCLUDE ${CMAKE_SOURCE_DIR}/external_libs/tclap-1.2.1/include)

add_definitions(${GLFW_CFLAGS_OTHER})


include_directories(${GLFW_INCLUDE_DIRS}
                    ${GLEW_INCLUDE_PATH}
                    ${LIBXML2_INCLUDE_DIR}
                    ${TCLAP_INCLUDE}
                    ${FREEIMAGE_INCLUDES})

link_directories(${GLFW_LIBRARY_DIRS})

IF(APPLE)
    SET(EXTRA_LIBS "-framework Cocoa -framework OpenGL")
ELSEIF(MSVC)
	SET(EXTRA_LIBS "glu32.lib" "opengl32.lib")
ELSE(APPLE)
    SET(EXTRA_LIBS ${GLFW_LDFLAGS_OTHER})
ENDIF(APPLE)

set(A3DViewer_HEADER src/App.h
                     src/Graphics.h
                     src/Mesh.h
                     src/ShaderUtils.h
                     src/ProgramSimple.h
                     src/Resources.h
                     src/Settings.h
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
                     src/Settings.cpp
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
                      ${FREEIMAGE_LIBS}
                      )
