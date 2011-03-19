
set(WizCommon_SOURCE
    Mesh.cpp
    MeshIO.cpp
    StringUtils.cpp
    FileUtils.cpp
    XmlParser.cpp
  )
set(WizCommon_INCLUDE
    WizIncludes.h
    WizTypes.h
    WizUtils.h
    Mesh.h
    MeshIO.h
    StringUtils.h
    FileUtils.h
    XmlParser.h
  )

set(Tinyxml_INCLUDE ${CMAKE_SOURCE_DIR}/external_libs/tinyxml)

include_directories(${Tinyxml_INCLUDE})

add_library(WizCommon ${WizCommon_SOURCE} ${WizCommon_INCLUDE})

target_link_libraries(WizCommon tinyxml)
