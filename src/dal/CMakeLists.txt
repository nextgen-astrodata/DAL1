
## ==============================================================================
##
##  libdal1: Source & header files
##
## ==============================================================================

foreach (dalModule
    core
    coordinates
    data_common
    data_hl
    )
  
  ##________________________________________________________
  ## List of library source files

  FILE (GLOB dal_${dalModule}_Sources ${dalModule}/*.cc)

  if (dal_${dalModule}_Sources)
    list (APPEND dalSources ${dal_${dalModule}_Sources})
  endif (dal_${dalModule}_Sources)

  ##________________________________________________________
  ## Installation of module header files
  
  FILE (GLOB dal_${dalModule}_Headers ${dalModule}/*.h)
  
  if (dal_${dalModule}_Headers)
    install (
      FILES ${dal_${dalModule}_Headers}
      DESTINATION ${DAL_INSTALL_INCLUDEDIR}/${dalModule}
      )
  endif (dal_${dalModule}_Headers)
  
  ##________________________________________________________
  ## Test programs for this module
  
  if (DAL_ENABLE_TESTING)
    add_subdirectory (${dalModule}/test)
  endif (DAL_ENABLE_TESTING)
  
endforeach (dalModule)

## ==============================================================================
##
##  libdal1: Compiler instructions
##
## ==============================================================================

if (DAL_SHARED_LIBRARY)
  add_library(dal1 SHARED ${dalSources})
else (DAL_SHARED_LIBRARY)
  add_library(dal1 STATIC ${dalSources})
endif (DAL_SHARED_LIBRARY)

set_target_properties (dal1
  PROPERTIES
  COMPILE_FLAGS "-fpermissive -ftemplate-depth-100"
  )

## ==============================================================================
##
##  libdal1: Linker instructions
##
## ==============================================================================

target_link_libraries(dal1 ${dal_link_libraries})

## ==============================================================================
##
##  libdal1: Installation instructions
##
## ==============================================================================

install (
  TARGETS dal1
  RUNTIME DESTINATION ${DAL_INSTALL_BINDIR}
  LIBRARY DESTINATION ${DAL_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${DAL_INSTALL_LIBDIR}
  )
