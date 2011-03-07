
## ==============================================================================
##
##  System inspection
##
## ==============================================================================

##____________________________________________________________________
##                                              Standard CMake modules

include (FindLATEX)
include (FindMPI)
include (FindOpenMP)

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
  list (APPEND _dalCMakeModules MySql)
endif (DAL_WITH_MYSQL)

foreach (_dalcmake ${_dalCMakeModules} )

  message (STATUS "Checking for package ${_dalcmake}")

  ## Generate uppercase version of package name
  string (TOUPPER ${_dalcmake} _dalcmake_var)
  ## Include CMake find script
  include (${DAL_SOURCE_DIR}/cmake/Find${_dalcmake}.cmake)

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
    message (STATUS "Adding compiler flag -DWORDSIZE_IS_64")
    add_definitions (-DWORDSIZE_IS_64)
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

## We need to be a bit careful with the Boost and Python libraries; appending
## the Boost libraries only makes sense, if there is already a list of Python
## libraries. Furthermore we need to make sure not to have libboost_python in
## in the list, because this requires for the presence of the Python library.

if (BOOST_LIBRARIES)
  if (PYTHON_LIBRARIES)
    list (APPEND PYTHON_LIBRARIES ${BOOST_LIBRARIES})
  else (PYTHON_LIBRARIES)
    ## Clean up the list of Boost libraries
    foreach (lib ${BOOST_LIBRARIES})
      string (REGEX MATCH ^.*python.* have_libboost_python ${lib})
      if (have_libboost_python)
	list (REMOVE_ITEM BOOST_LIBRARIES ${have_libboost_python})
      endif (have_libboost_python)
    endforeach (lib)
    ## Add the libraries
    set (PYTHON_LIBRARIES ${BOOST_LIBRARIES})
  endif (PYTHON_LIBRARIES)
endif (BOOST_LIBRARIES)

## Effect of dependencies on build options; if we are unable to locate all the
## required components to generate the Python bindings to the DAL, we have to 
## disable this option - otherwise we won't be able to properly build the test
## programs and applications.

if (NOT HAVE_BOOST)
  set (DAL_PYTHON_BINDINGS FALSE)
  message (STATUS "[DAL] Unable to generate Python bindings; missing Boost++ headers!")
endif (NOT HAVE_BOOST)

if (NOT HAVE_PYTHON) 
  set (DAL_PYTHON_BINDINGS FALSE)
  message (STATUS "[DAL] Unable to generate Python bindings; missing Python headers!")
endif (NOT HAVE_PYTHON)

if (NOT NUMUTIL_INCLUDES) 
  set (DAL_PYTHON_BINDINGS FALSE)
  message (STATUS "[DAL] Unable to generate Python bindings; missing num_util headers!")
endif (NOT NUMUTIL_INCLUDES)

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
