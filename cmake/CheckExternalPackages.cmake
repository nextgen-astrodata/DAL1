
## ==============================================================================
##
##  System inspection
##
## ==============================================================================

## Adjust the module path 

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

##____________________________________________________________________
##                                              Standard CMake modules

include (FindLATEX)
include (FindMPI)
include (FindOpenMP)

if (MPI_INCLUDE_PATH)
  include_directories (${MPI_INCLUDE_PATH})
endif (MPI_INCLUDE_PATH)

##____________________________________________________________________
##                                                Custom CMake modules

set (_dalCMakeModules
  Boost
  Casacore
  CFITSIO
  GFortran
  GSL
  HDF5
  LAPACK
  LOFAR
  NumPy
  NumUtil
  Python
  WCSLIB
  )

if (DAL_WITH_MYSQL)
  list (APPEND _dalCMakeModules MySQL)
endif (DAL_WITH_MYSQL)

foreach (_dalcmake ${_dalCMakeModules} )

  message (STATUS "Checking for package ${_dalcmake}")

  ## Generate uppercase version of package name
  string (TOUPPER ${_dalcmake} _dalcmake_var)
  ## Initilize CMake varible indicating wether or not package was found
  set (${_dalcmake_var}_FOUND FALSE)
  ## Include CMake find script
  include (Find${_dalcmake})

  if (${_dalcmake_var}_FOUND OR HAVE_${_dalcmake_var})

    ## variable mapping
    set (HAVE_${_dalcmake_var}     TRUE)
    set (DAL_WITH_${_dalcmake_var} TRUE)
    
    ## include directories
    include_directories (${${_dalcmake_var}_INCLUDES})

    message (STATUS "Checking for package ${_dalcmake} - Success")

  endif (${_dalcmake_var}_FOUND OR HAVE_${_dalcmake_var})

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
