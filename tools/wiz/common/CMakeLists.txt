
set(A3DTools_SOURCE
    Mesh.cpp
    MeshIO.cpp
    StringUtils.cpp
    FileUtils.cpp
    XmlParser.cpp
  )
set(A3DTools_HEADER
    Assembly3D.h
    A3DIncludes.h
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

add_library(A3DTools ${A3DTools_SOURCE} ${A3DTools_HEADER}
                      ${Tinyxml_SOURCE} ${Tinyxml_INCLUDE})

install(TARGETS A3DTools
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

install(FILES
Assembly3D.h
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
