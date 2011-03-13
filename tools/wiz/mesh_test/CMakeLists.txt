
set(TCLAP_INCLUDE ${CMAKE_SOURCE_DIR}/external_libs/tclap-1.2.0/include)
set(Tinyxml_INCLUDE ${CMAKE_SOURCE_DIR}/external_libs/tinyxml)
set(WizUtils_INCLUDE ${CMAKE_SOURCE_DIR}/common)

include_directories(${WizUtils_INCLUDE}
                    ${Tinyxml_INCLUDE}
                    ${TCLAP_INCLUDE})

add_executable(MeshTester Main.cpp
                          TesterTool.h
                          TesterTool.cpp
                          MeshTesterIncludes.h
                          MeshTesterTypes.h)

target_link_libraries(MeshTester WizUtils)
