
set(WizCommon_SOURCE
    StringUtils.cpp
    FileUtils.cpp
    XmlParser.cpp
  )
set(WizCommon_INCLUDE
    WizIncludes.h
    StringUtils.h
    FileUtils.h
    XmlParser.h
  )

set(Tinyxml_INCLUDE ${CMAKE_SOURCE_DIR}/external_libs/tinyxml)

include_directories(${Tinyxml_INCLUDE})

add_library(WizCommon ${WizCommon_SOURCE} ${WizCommon_INCLUDE})

target_link_libraries(WizCommon tinyxml)
