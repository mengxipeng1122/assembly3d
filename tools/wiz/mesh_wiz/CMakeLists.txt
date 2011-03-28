set(TCLAP_INCLUDE ${CMAKE_SOURCE_DIR}/external_libs/tclap-1.2.0/include)
set(Tinyxml_INCLUDE ${CMAKE_SOURCE_DIR}/external_libs/tinyxml)
set(A3DCommon_INCLUDE ${CMAKE_SOURCE_DIR}/common)

set(MeshWiz_HEADERS
    MeshWizIncludes.h
    AttributeTool.h
    ConvertTool.h
    OptimizeTool.h
    RenameTool.h
    ToolManager.h
    TransformTool.h
    FrontFaceTool.h
    )

set(MeshWiz_SOURCE
    AttributeTool.cpp
    ConvertTool.cpp
    OptimizeTool.cpp
    RenameTool.cpp
    ToolManager.cpp
    TransformTool.cpp
    FrontFaceTool.cpp
    )

include_directories(${A3DCommon_INCLUDE} ${Tinyxml_INCLUDE} ${TCLAP_INCLUDE})

add_library(MeshWiz_lib STATIC ${MeshWiz_SOURCE} ${MeshWiz_HEADERS})
set_target_properties(MeshWiz_lib PROPERTIES OUTPUT_NAME MeshWiz)
target_link_libraries(MeshWiz_lib A3DCommon)

add_executable(MeshWiz Main.cpp)
#set_target_properties(MeshWiz_bin PROPERTIES OUTPUT_NAME MeshWiz)

target_link_libraries(MeshWiz MeshWiz_lib A3DCommon)

if(WIN32)
    install(TARGETS MeshWiz
        RUNTIME DESTINATION .)
else()
    install(TARGETS MeshWiz
        RUNTIME DESTINATION bin)
endif()
