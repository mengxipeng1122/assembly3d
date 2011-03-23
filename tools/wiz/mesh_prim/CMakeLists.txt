set(TCLAP_INCLUDE ${CMAKE_SOURCE_DIR}/external_libs/tclap-1.2.0/include)
set(Tinyxml_INCLUDE ${CMAKE_SOURCE_DIR}/external_libs/tinyxml)
set(A3DCommon_INCLUDE ${CMAKE_SOURCE_DIR}/common)

include_directories(${A3DCommon_INCLUDE}
                    ${Tinyxml_INCLUDE}
                    ${TCLAP_INCLUDE})

set(MeshPrim_SOURCE Main.cpp
                    MeshPrimIncludes.h
                    PrimGen.h
                    PrimGen.cpp
                    Primitive.h
                    primitives/Plane.h
                    primitives/Plane.cpp
                    primitives/Rectangle.h
                    primitives/Rectangle.cpp
                    primitives/Cube.h
                    primitives/Cube.cpp
                    primitives/Sphere.h
                    primitives/Sphere.cpp
                    primitives/Torus.h
                    primitives/Torus.cpp
                    primitives/Trapezoid.h
                    primitives/Trapezoid.cpp
                    primitives/Cylinder.h
                    primitives/Cylinder.cpp
                    primitives/Disk.h
                    primitives/Disk.cpp
                    primitives/PartialCylinder.h
                    primitives/PartialCylinder.cpp
                    primitives/PartialDisk.h
                    primitives/PartialDisk.cpp)

add_executable(MeshPrim ${MeshPrim_SOURCE})

target_link_libraries(MeshPrim A3DCommon)

if(WIN32)
    install(TARGETS MeshPrim RUNTIME DESTINATION .)
else()
    install(TARGETS MeshPrim RUNTIME DESTINATION bin)
endif()
