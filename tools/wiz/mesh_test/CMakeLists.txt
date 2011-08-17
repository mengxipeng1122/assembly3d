
set(TCLAP_INCLUDE ${CMAKE_SOURCE_DIR}/external_libs/tclap-1.2.0/include)
set(A3DTools_INCLUDE ${CMAKE_SOURCE_DIR}/common)

include_directories(${A3DTools_INCLUDE} ${TCLAP_INCLUDE})

set(MeshTest_HEADER
    MeshTest.h
    TesterTool.h
    MeshTestIncludes.h
    MeshTestTypes.h)

set(MeshTest_SOURCE TesterTool.cpp)

add_library(MeshTest_lib ${MeshTest_HEADER} ${MeshTest_SOURCE})
set_target_properties(MeshTest_lib PROPERTIES OUTPUT_NAME MeshTest)

add_executable(MeshTest_bin Main.cpp)
set_target_properties(MeshTest_bin PROPERTIES OUTPUT_NAME MeshTest)
target_link_libraries(MeshTest_bin MeshTest_lib A3DTools)

if(WIN32)
    install(TARGETS MeshTest_bin MeshTest_lib
        RUNTIME DESTINATION .
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
else()
    install(TARGETS MeshTest_bin MeshTest_lib
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
endif()

install(FILES
MeshTest.h
MeshTestTypes.h
TesterTool.h
DESTINATION ${CMAKE_INSTALL_PREFIX}/include/a3dtools/include)
