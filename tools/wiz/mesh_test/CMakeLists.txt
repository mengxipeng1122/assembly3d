
set(TCLAP_INCLUDE ${CMAKE_SOURCE_DIR}/external_libs/tclap-1.2.0/include)
set(Tinyxml_INCLUDE ${CMAKE_SOURCE_DIR}/external_libs/tinyxml)
set(A3DCommon_INCLUDE ${CMAKE_SOURCE_DIR}/common)

include_directories(${A3DCommon_INCLUDE} ${Tinyxml_INCLUDE} ${TCLAP_INCLUDE})

add_executable(MeshTest Main.cpp
                        TesterTool.h
                        TesterTool.cpp
                        MeshTestIncludes.h
                        MeshTestTypes.h)

target_link_libraries(MeshTest A3DCommon)

if(WIN32)
    install(TARGETS MeshTest RUNTIME DESTINATION .)
else()
    install(TARGETS MeshTest RUNTIME DESTINATION bin)
endif()
