
set(WizUtils_SOURCE
    StringUtils.cpp
    FileUtils.cpp
    XmlParser.cpp
  )
set(WizUtils_INCLUDE
    StringUtils.h
    FileUtils.h
    XmlParser.h
  )

set(Tinyxml_INCLUDE ${CMAKE_SOURCE_DIR}/external_libs/tinyxml)

include_directories(${Tinyxml_INCLUDE})

add_library(WizUtils ${WizUtils_SOURCE} ${WizUtils_INCLUDE})

target_link_libraries(WizUtils tinyxml)
