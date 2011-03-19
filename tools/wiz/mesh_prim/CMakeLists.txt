set(TCLAP_INCLUDE ${CMAKE_SOURCE_DIR}/external_libs/tclap-1.2.0/include)
set(Tinyxml_INCLUDE ${CMAKE_SOURCE_DIR}/external_libs/tinyxml)
set(WizCommon_INCLUDE ${CMAKE_SOURCE_DIR}/common)

include_directories(${WizCommon_INCLUDE}
                    ${Tinyxml_INCLUDE}
                    ${TCLAP_INCLUDE})

add_executable(MeshPrim Main.cpp
                        MeshPrimIncludes.h
                        PrimGen.h
                        PrimGen.cpp
                        Primitive.h
                        primitives/Plane.h
                        primitives/Plane.cpp
                        primitives/Cube.h
                        primitives/Cube.cpp
                        primitives/Sphere.h
                        primitives/Sphere.cpp
                        primitives/Torus.h
                        primitives/Torus.cpp)

target_link_libraries(MeshPrim WizCommon)

#install(TARGETS MeshTest DESTINATION ${CMAKE_SOURCE_DIR}/testsuite)
if(WIN32)
    install(TARGETS MeshPrim RUNTIME DESTINATION .)
else()
    install(TARGETS MeshPrim RUNTIME DESTINATION bin)
endif()
