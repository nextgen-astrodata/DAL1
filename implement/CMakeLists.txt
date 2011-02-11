##------------------------------------------------------------------------------
## $Id::                                                                       $
##------------------------------------------------------------------------------

## ==============================================================================
##
##  Compiler flags
##
## ==============================================================================

if (HAVE_CASA OR CASACORE_FOUND)
  IF (APPLE)
    ## definition common to apple architectures
    add_definitions (
      -DAIPS_DARWIN
      )
  ELSE (APPLE)
    ADD_DEFINITIONS (-DAIPS_LINUX)
  ENDIF (APPLE)
  ## Platform test 32/64 bit ------------------------------
  set (CMAKE_SYSTEM_64BIT 0)
  if (NOT CMAKE_SYSTEM_PROCESSOR MATCHES i386)
    if (NOT CMAKE_SYSTEM_PROCESSOR MATCHES i686)
      if (NOT CMAKE_SYSTEM_PROCESSOR MATCHES powerpc)
        set (CMAKE_SYSTEM_64BIT 1)
        add_definitions (-DAIPS_64B)
      endif (NOT CMAKE_SYSTEM_PROCESSOR MATCHES powerpc)
    endif (NOT CMAKE_SYSTEM_PROCESSOR MATCHES i686)
  endif (NOT CMAKE_SYSTEM_PROCESSOR MATCHES i386)
  if (APPLE)
    if (NOT CMAKE_SYSTEM_PROCESSOR MATCHES powerpc)
      set (CMAKE_SYSTEM_64BIT 1)
      add_definitions (-DAIPS_64B)
    endif (NOT CMAKE_SYSTEM_PROCESSOR MATCHES powerpc)
  endif (APPLE)
endif (HAVE_CASA OR CASACORE_FOUND)

## ==============================================================================
##
##  Build the library, "libdal"
##
## ==============================================================================

##__________________________________________________________
## Define the list of modules

set (dal_modules
  core
  coordinates
  data_common
  data_hl
  )

##__________________________________________________________
## Lists of source and header files

file (GLOB dal_headers *.h)

##__________________________________________________________
## Process the contents of the modules directories

foreach (dal_module ${dal_modules})
  ## Add include paths
  include_directories (${DAL_SOURCE_DIR}/implement/${dal_module})
  ## list of source files for this module
  FILE (GLOB dal_${dal_module}_sources ${dal_module}/*.cc ${dal_module}/*.tcc)
  ## list of header files for this module
  FILE (GLOB dal_${dal_module}_headers ${dal_module}/*.h)
  ## expand the list of source files
  list (APPEND dal_sources ${dal_${dal_module}_sources})
  list (APPEND dal_headers ${dal_${dal_module}_headers})
endforeach (dal_module)

##____________________________________________________________________
## Build the library

add_library(dal STATIC ${dal_sources})

target_link_libraries(dal ${dal_link_libraries})

set_target_properties (dal
  PROPERTIES
  COMPILE_FLAGS "-fpermissive -ftemplate-depth-100"
  )

## ==============================================================================
##
## Build the library for the Python bindings, "pydal"
##
## ==============================================================================

if (DAL_PYTHON_BINDINGS)
  add_subdirectory (bindings)
endif (DAL_PYTHON_BINDINGS)

## ==============================================================================
##
##  Installation instructions
##
## ==============================================================================

## installation of libraries

install (
  TARGETS dal
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  )

## installation of header files

install (FILES ${dal_headers}
  DESTINATION include/dal
  )
