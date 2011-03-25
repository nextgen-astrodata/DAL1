
## ==============================================================================
##
##  System inspection
##
## ==============================================================================

##____________________________________________________________________
##                                                  CMake find modules

## Adjust the module path 
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

## List of modules to be loaded in
set (_cmakeModules
  Boost_DAL
  Casacore
  CFITSIO_DAL
  GFortran
  GSL_DAL
  HDF5_DAL
  LAPACK_DAL
  LATEX
  LOFAR
  MPI
  NumPy
  NumUtil
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
  ## Strip "_DAL"
  string(REPLACE "_DAL" "" _cmakeModuleVariable ${_dalcmake_var})
  ## Initilize CMake varible indicating wether or not package was found
  set (${_dalcmake_var}_FOUND FALSE)
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

##____________________________________________________________________
##                                          System libraries and tools

find_library (DL_LIBRARY
  NAMES dl
  HINTS ${CMAKE_INSTALL_PREFIX} 
  PATHS /sw /usr /usr/local /opt/local
  PATH_SUFFIXES lib
  )

find_library (M_LIBRARY
  NAMES m
  HINTS ${CMAKE_INSTALL_PREFIX} 
  PATHS /sw /usr /usr/local /opt/local
  PATH_SUFFIXES lib
  )

find_library (PTHREAD_LIBRARY
  NAMES pthread
  HINTS ${CMAKE_INSTALL_PREFIX} 
  PATHS /sw /usr /usr/local /opt/local
  PATH_SUFFIXES lib
  )

find_library (UTIL_LIBRARY
  NAMES util
  HINTS ${CMAKE_INSTALL_PREFIX} 
  PATHS /sw /usr /usr/local /opt/local
  PATH_SUFFIXES lib
  )

find_library (Z_LIBRARY
  NAMES z
  HINTS ${CMAKE_INSTALL_PREFIX} 
  PATHS /sw /usr /usr/local /opt/local
  PATH_SUFFIXES lib
  )

## ==============================================================================
##
##  Python bindings
##
## ==============================================================================

if (DAL_PYTHON_BINDINGS)
  include (${DAL_SOURCE_DIR}/cmake/TestPythonBindings.cmake)
endif (DAL_PYTHON_BINDINGS)

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
