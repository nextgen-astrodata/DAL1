
## ==============================================================================
##
##  Build the library, "libdal"
##
## ==============================================================================

##__________________________________________________________
## Assemble list of source files

FILE (GLOB dal_sources
  core/*.cc
  coordinates/*.cc
  data_common/*.cc
  data_hl/*.cc
  )

FILE (GLOB dal_headers
  core/*.h
  coordinates/*.h
  data_common/*.h
  data_hl/*.h
  )

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
