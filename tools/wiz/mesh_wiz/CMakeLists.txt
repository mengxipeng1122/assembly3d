
set(TCLAP_INCLUDE ${CMAKE_SOURCE_DIR}/external_libs/tclap-1.2.0/include)
set(Tinyxml_INCLUDE ${CMAKE_SOURCE_DIR}/external_libs/tinyxml)
set(A3DCommon_INCLUDE ${CMAKE_SOURCE_DIR}/common)

include_directories(${A3DCommon_INCLUDE}
                    ${Tinyxml_INCLUDE}
                    ${TCLAP_INCLUDE})

file(GLOB MeshWiz_SOURCE *.cpp)
file(GLOB MeshWiz_INCLUDE *.h)


add_executable(MeshWiz ${MeshWiz_SOURCE}
                       ${MeshWiz_INCLUDE})

target_link_libraries(MeshWiz A3DCommon)

if(WIN32)
	install(TARGETS MeshWiz RUNTIME DESTINATION .)
else()
	install(TARGETS MeshWiz RUNTIME DESTINATION bin)
endif()
