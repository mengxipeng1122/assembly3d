
set(A3DCommon_SOURCE
    Mesh.cpp
    MeshIO.cpp
    StringUtils.cpp
    FileUtils.cpp
    XmlParser.cpp
  )
set(A3DCommon_HEADER
    Assembly3D.h
    A3DIncludes.h
    A3DTypes.h
    A3DUtils.h
    Mesh.h
    MeshIO.h
    StringUtils.h
    FileUtils.h
    XmlParser.h
  )

set(Tinyxml_SOURCE
  tinyxml/tinyxml.cpp
  tinyxml/tinyxmlerror.cpp
  tinyxml/tinyxmlparser.cpp
  )
set(Tinyxml_INCLUDE
  tinyxml/tinyxml.h
  )

add_library(A3DCommon ${A3DCommon_SOURCE} ${A3DCommon_HEADER}
                      ${Tinyxml_SOURCE} ${Tinyxml_INCLUDE})

install(TARGETS A3DCommon
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

install(FILES
Assembly3D.h
A3DTypes.h
A3DUtils.h
Mesh.h
MeshIO.h
StringUtils.h
FileUtils.h
XmlParser.h
DESTINATION ${CMAKE_INSTALL_PREFIX}/include/a3dtools/include)

install(FILES
${Tinyxml_INCLUDE}
DESTINATION ${CMAKE_INSTALL_PREFIX}/include/a3dtools/include/tinyxml)
