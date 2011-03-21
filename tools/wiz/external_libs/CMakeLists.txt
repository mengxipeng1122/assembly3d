
set(Tinyxml_SOURCE
  tinyxml/tinyxml.cpp
  tinyxml/tinystr.cpp
  tinyxml/tinyxmlerror.cpp
  tinyxml/tinyxmlparser.cpp
  )
set(Tinyxml_INCLUDE
  tinyxml/tinyxml.h
  tinyxml/tinystr.h
  )

add_library(tinyxml ${Tinyxml_SOURCE} ${Tinyxml_INCLUDE})

