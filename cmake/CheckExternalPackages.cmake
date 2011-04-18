
## ==============================================================================
##
##  System inspection
##
## ==============================================================================

##____________________________________________________________________
## Locations to search for required external components

set (DAL_FIND_PATHS
  /sw
  /usr
  /usr/local
  /opt
  /opt/local
  ${CMAKE_INSTALL_PREFIX}
)

##____________________________________________________________________
##                                          System libraries and tools

foreach (_syslib dl m pthread util z)

  string (TOUPPER ${_syslib} _syslibVar)

  find_library (${_syslibVar}_LIBRARY
    NAMES ${_syslib}
    PATHS ${DAL_FIND_PATHS}
    PATH_SUFFIXES lib
    )
  
endforeach (_syslib)

##____________________________________________________________________
##                                                  CMake find modules

## Adjust the module path 
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

## List of modules to be loaded in
set (_cmakeModules
  Boost_DAL
  Casacore
  CFITSIO_DAL
  Doxygen
  GFortran
  GSL_DAL
  HDF5_DAL
  LAPACK_DAL
  LATEX_DAL
  LOFAR
  MPI
  NumPy
  OpenMP
  Python_DAL
  WCSLIB_DAL
  )

if (DAL_WITH_MYSQL)
  list (APPEND _cmakeModules MySQL)
endif (DAL_WITH_MYSQL)

foreach (_dalcmake ${_cmakeModules} )

  message (STATUS "Checking for package ${_dalcmake}")

  ## Generate uppercase version of package name
  string (TOUPPER ${_dalcmake} _dalcmake_var)
  ## Strip "_DAL" from module name for usage as internal variable
  string(REPLACE "_DAL" "" _cmakeModuleVariable ${_dalcmake_var})
  ## Initilize CMake varible indicating wether or not package was found
  set (${_dalcmake_var}_FOUND FALSE)
  ## Enable/disable verbosity mode
  if (DAL_VERBOSE_CONFIGURE)
    set (${_cmakeModuleVariable}_FIND_QUIETLY NO )
    set (${_dalcmake}_FIND_QUIETLY            NO )
  else (DAL_VERBOSE_CONFIGURE)
    set (${_cmakeModuleVariable}_FIND_QUIETLY YES )
    set (${_dalcmake}_FIND_QUIETLY            YES )
  endif (DAL_VERBOSE_CONFIGURE)
  ## Include CMake find script
  include (Find${_dalcmake})
  
  if (${_cmakeModuleVariable}_FOUND OR HAVE_${_cmakeModuleVariable})

    ## variable mapping
    set (HAVE_${_cmakeModuleVariable}      TRUE )
    set (${_cmakeModuleVariable}_FOUND     TRUE )
    set (DAL_WITH_${_cmakeModuleVariable}  TRUE )
    
    ## include directories
    include_directories (${${_cmakeModuleVariable}_INCLUDES})
    if (${_cmakeModuleVariable}_INCLUDE_PATH)
      include_directories (${${_cmakeModuleVariable}_INCLUDE_PATH})
    endif (${_cmakeModuleVariable}_INCLUDE_PATH)
    
    message (STATUS "Checking for package ${_dalcmake} - Success")
    
  else (${_cmakeModuleVariable}_FOUND OR HAVE_${_cmakeModuleVariable})
  
    message (STATUS "Checking for package ${_dalcmake} - FAIL")

  endif (${_cmakeModuleVariable}_FOUND OR HAVE_${_cmakeModuleVariable})
  
endforeach (_dalcmake)

##____________________________________________________________________
##                                                          Type sizes

if (SIZEOF_LONG_LONG)
  add_definitions (-DHAVE_LONG_LONG)
endif (SIZEOF_LONG_LONG)

if (CMAKE_SIZEOF_VOID_P)
  if (${CMAKE_SIZEOF_VOID_P} EQUAL 8)
    set (WORDSIZE_IS_64 TRUE)
  else (${CMAKE_SIZEOF_VOID_P} EQUAL 8)
    set (WORDSIZE_IS_64 FALSE)
  endif (${CMAKE_SIZEOF_VOID_P} EQUAL 8)
endif (CMAKE_SIZEOF_VOID_P)

## ==============================================================================
##
##  Build options
##
## ==============================================================================

if (NOT BOOST_FOUND)
  set (DAL_PYTHON_BINDINGS FALSE)
endif (NOT BOOST_FOUND)

if (NOT DOXYGEN_FOUND)
  set (DAL_WITH_DOCUMENTATION FALSE)
  message (STATUS "Skip generating documentstion - Doxygen not found!")
endif (NOT DOXYGEN_FOUND)

## ==============================================================================
##
##  Configure file
##
##  Any occurrences of #cmakedefine VAR will be replaced with either #define VAR
##  or /* #undef VAR */ depending on the setting of VAR in CMake. Any occurrences
##  of #cmakedefine01 VAR will be replaced with either #define VAR 1 or
##  #define VAR 0 depending on whether VAR evaluates to TRUE or FALSE in CMake
##
## ==============================================================================

configure_file (
  ${DAL_SOURCE_DIR}/cmake/dal_config.h.in
  ${DAL_BINARY_DIR}/dal_config.h
  )
