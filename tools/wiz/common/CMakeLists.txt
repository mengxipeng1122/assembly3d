
set(Assembly3DCommon_SOURCE
    Mesh.cpp
    MeshIO.cpp
    StringUtils.cpp
    FileUtils.cpp
    XmlParser.cpp
  )
set(Assembly3DCommon_INCLUDE
    Assembly3DIncludes.h
    Assembly3DTypes.h
    Assembly3DUtils.h
    Mesh.h
    MeshIO.h
    StringUtils.h
    FileUtils.h
    XmlParser.h
  )

set(Tinyxml_INCLUDE ${CMAKE_SOURCE_DIR}/external_libs/tinyxml)

include_directories(${Tinyxml_INCLUDE})

add_library(Assembly3DCommon ${Assembly3DCommon_SOURCE}
                             ${Assembly3DCommon_INCLUDE})

target_link_libraries(Assembly3DCommon tinyxml)
