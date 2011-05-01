
## ==============================================================================
##
##  Build the library, "libdal"
##
## ==============================================================================

##__________________________________________________________
## Process the contents of the modules directories
##
##  .
##  ├── coordinates
##  ├── core
##  ├── data_common
##  └── data_hl
##

foreach (dal_module
    core
    coordinates
    data_common
    data_hl
    )
  
  ## list of source files for this module
  FILE (GLOB dal_${dal_module}_sources ${dal_module}/*.cc)
  
  ## expand the list of source files
  list (APPEND dal_sources ${dal_${dal_module}_sources})

  ## Installation of module header files

  FILE (GLOB dal_${dal_module}_headers ${dal_module}/*.h)
  
  install (FILES ${dal_${dal_module}_headers}
    DESTINATION ${DAL_INSTALL_INCLUDEDIR}/${dal_module}
    )
  
endforeach (dal_module)

##____________________________________________________________________
## libdal: Compiler instructions

if (DAL_SHARED_LIBRARY)
  add_library(dal SHARED ${dal_sources})
else (DAL_SHARED_LIBRARY)
  add_library(dal STATIC ${dal_sources})
endif (DAL_SHARED_LIBRARY)

set_target_properties (dal
  PROPERTIES
  COMPILE_FLAGS "-fpermissive -ftemplate-depth-100"
  )

##____________________________________________________________________
## libdal: Linker instructions

target_link_libraries(dal ${dal_link_libraries})

##____________________________________________________________________
## libdal: Installation instructions

install (
  TARGETS dal
  RUNTIME DESTINATION ${DAL_INSTALL_BINDIR}
  LIBRARY DESTINATION ${DAL_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${DAL_INSTALL_LIBDIR}
  )
