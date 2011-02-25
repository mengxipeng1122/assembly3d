
set(WizUtils_SOURCE
    wiz_utils/StringUtils.cpp
    wiz_utils/FileUtils.cpp
    wiz_utils/XmlParser.cpp
  )
set(WizUtils_INCLUDE
    WizUtils.h
    WizIncludes.h
    wiz_utils/StringUtils.h
    wiz_utils/FileUtils.h
    wiz_utils/XmlParser.h
  )

set(Tinyxml_INCLUDE ${CMAKE_SOURCE_DIR}/external_libs/tinyxml)

include_directories(${Tinyxml_INCLUDE})

add_library(WizUtils ${WizUtils_SOURCE} ${WizUtils_INCLUDE})

target_link_libraries(WizUtils tinyxml)
