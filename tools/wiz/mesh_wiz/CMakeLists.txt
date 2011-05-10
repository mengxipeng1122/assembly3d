
set(TCLAP_INCLUDE ${CMAKE_SOURCE_DIR}/external_libs/tclap-1.2.0/include)
set(A3DTools_INCLUDE ${CMAKE_SOURCE_DIR}/common)

set(MeshWiz_HEADERS
    MeshWiz.h
    MeshWizIncludes.h
    AttributeTool.h
    ConvertTool.h
    OptimizeTool.h
    RenameTool.h
    ToolManager.h
    TransformTool.h
    FrontFaceTool.h
    TextureTool.h
    )

set(MeshWiz_SOURCE
    AttributeTool.cpp
    ConvertTool.cpp
    OptimizeTool.cpp
    RenameTool.cpp
    ToolManager.cpp
    TransformTool.cpp
    FrontFaceTool.cpp
    TextureTool.cpp
    )

include_directories(${A3DTools_INCLUDE} ${TCLAP_INCLUDE})

add_library(MeshWiz_lib STATIC ${MeshWiz_SOURCE} ${MeshWiz_HEADERS})
set_target_properties(MeshWiz_lib PROPERTIES OUTPUT_NAME MeshWiz)
#target_link_libraries(MeshWiz_lib A3DTools)

add_executable(MeshWiz_bin Main.cpp)
set_target_properties(MeshWiz_bin PROPERTIES OUTPUT_NAME MeshWiz)

target_link_libraries(MeshWiz_bin MeshWiz_lib A3DTools)

if(WIN32)
    install(TARGETS MeshWiz_bin MeshWiz_lib
        RUNTIME DESTINATION .
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
else()
    install(TARGETS MeshWiz_bin MeshWiz_lib
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
endif()

install(FILES
MeshWiz.h
AttributeTool.h
ConvertTool.h
OptimizeTool.h
RenameTool.h
ToolManager.h
TransformTool.h
FrontFaceTool.h
DESTINATION ${CMAKE_INSTALL_PREFIX}/include/a3dtools/include)
