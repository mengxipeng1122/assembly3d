
set(Tinyxml_SOURCE
  tinyxml/tinyxml.cpp
  tinyxml/tinyxmlerror.cpp
  tinyxml/tinyxmlparser.cpp
  )
set(Tinyxml_INCLUDE
  tinyxml/tinyxml.h
  )

add_library(tinyxml ${Tinyxml_SOURCE} ${Tinyxml_INCLUDE})

install(TARGETS tinyxml
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

install(FILES
${Tinyxml_INCLUDE}
DESTINATION ${CMAKE_INSTALL_PREFIX}/include/a3dtools/include)
