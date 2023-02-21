
file(GLOB libdebug_mgr_SRC
  "*.h"
  "*.cpp"
  )
  
add_library(debug-mgr SHARED ${libdebug_mgr_SRC})

target_include_directories(debug-mgr PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/include
    )
   
#add_library(debug-mgr_static STATIC ${libdebug_mgr_SRC})
#
#target_include_directories(debug-mgr_static PUBLIC
#    ${PROJECT_SOURCE_DIR}/src
#    ${PROJECT_SOURCE_DIR}/src/include
#    )
   
#install(TARGETS debug-mgr_static
#    DESTINATION lib
#    EXPORT debug-mgr-targets)

install(TARGETS debug-mgr
    DESTINATION lib
    EXPORT debug-mgr-targets)


