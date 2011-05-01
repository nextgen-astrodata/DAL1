
## ==============================================================================
##
##  libdal: Source & header files
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
  
endforeach (dalModule)

## ==============================================================================
##
##  libdal: Compiler instructions
##
## ==============================================================================

if (DAL_SHARED_LIBRARY)
  add_library(dal SHARED ${dalSources})
else (DAL_SHARED_LIBRARY)
  add_library(dal STATIC ${dalSources})
endif (DAL_SHARED_LIBRARY)

set_target_properties (dal
  PROPERTIES
  COMPILE_FLAGS "-fpermissive -ftemplate-depth-100"
  )

## ==============================================================================
##
##  libdal: Linker instructions
##
## ==============================================================================

target_link_libraries(dal ${dal_link_libraries})

## ==============================================================================
##
##  libdal: Installation instructions
##
## ==============================================================================

install (
  TARGETS dal
  RUNTIME DESTINATION ${DAL_INSTALL_BINDIR}
  LIBRARY DESTINATION ${DAL_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${DAL_INSTALL_LIBDIR}
  )
