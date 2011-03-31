set(TCLAP_INCLUDE ${CMAKE_SOURCE_DIR}/external_libs/tclap-1.2.0/include)
set(A3DCommon_INCLUDE ${CMAKE_SOURCE_DIR}/common)

include_directories(${A3DCommon_INCLUDE} ${TCLAP_INCLUDE})

set(MeshPrim_HEADER MeshPrim.h
                    MeshPrimIncludes.h
                    PrimGen.h
                    Primitive.h
                    primitives/Plane.h
                    primitives/Rectangle.h
                    primitives/Cube.h
                    primitives/Sphere.h
                    primitives/Torus.h
                    primitives/Trapezoid.h
                    primitives/Cylinder.h
                    primitives/Torus.cpp
                    primitives/Disk.h
                    primitives/PartialCylinder.h
                    primitives/PartialDisk.h)

set(MeshPrim_SOURCE PrimGen.cpp
                    primitives/Plane.cpp
                    primitives/Rectangle.cpp
                    primitives/Cube.cpp
                    primitives/Sphere.cpp
                    primitives/Trapezoid.cpp
                    primitives/Cylinder.cpp
                    primitives/Disk.cpp
                    primitives/PartialCylinder.cpp
                    primitives/PartialDisk.cpp)

add_library(MeshPrim_lib ${MeshPrim_SOURCE} ${MeshPrim_HEADER})
set_target_properties(MeshPrim_lib PROPERTIES OUTPUT_NAME MeshPrim)
target_link_libraries(MeshPrim_lib)

add_executable(MeshPrim_bin Main.cpp)
set_target_properties(MeshPrim_bin PROPERTIES OUTPUT_NAME MeshPrim)
target_link_libraries(MeshPrim_bin MeshPrim_lib A3DCommon)

if(WIN32)
    install(TARGETS MeshPrim_bin MeshPrim_lib
        RUNTIME DESTINATION .
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
else()
    install(TARGETS MeshPrim_bin MeshPrim_lib
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
endif()

install(FILES
MeshPrim.h
MeshPrimIncludes.h
PrimGen.h
Primitive.h
DESTINATION ${CMAKE_INSTALL_PREFIX}/include/a3dtools)

install(FILES
primitives/Plane.h
primitives/Rectangle.h
primitives/Cube.h
primitives/Sphere.h
primitives/Torus.h
primitives/Trapezoid.h
primitives/Cylinder.h
primitives/Torus.cpp
primitives/Disk.h
primitives/PartialCylinder.h
primitives/PartialDisk.h
DESTINATION ${CMAKE_INSTALL_PREFIX}/include/a3dtools/primitives)

