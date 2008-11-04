##------------------------------------------------------------------------------
## $Id:: CMakeLists.txt                                                        $
##------------------------------------------------------------------------------

## Name of the project
project (DAL)

## Minimum required version of CMake to configure the project
cmake_minimum_required (VERSION 2.5)

## ==============================================================================
##
##  CMake configuration settings
##
## ==============================================================================

find_path (USG_ROOT devel_common/cmake/CMakeSettings.cmake
  PATHS 
  ${DAL_SOURCE_DIR}
  ${DAL_SOURCE_DIR}/..
  ${DAL_SOURCE_DIR}/../..
  ${DAL_SOURCE_DIR}/../../..
  $ENV{LOFARSOFT}
  )

if (USG_ROOT)
  include (${USG_ROOT}/devel_common/cmake/CMakeSettings.cmake)
else (USG_ROOT)
  message (FATAL_ERROR "Unable to locate additional CMake scripts!")
endif (USG_ROOT)

## ==============================================================================
##
##  Options
##
## ==============================================================================

option (DAL_BUILD_TESTS        "Build the test programs?"                  YES )
option (DAL_BUILD_APPS         "Build the applications?"                   YES )
option (DAL_ENABLE_TESTING     "Build the test programs?"                  YES )
option (DAL_COMPILER_WARNINGS  "Enable standard set of compiler warnings?" ON  )
option (DAL_MEMORY_CHECKING    "Enable checking against memory leaks?"     NO  )
option (DAL_PYTHON_BINDINGS    "Create python bindings?"                   YES )
option (DAL_DEBUGGING_MESSAGES "Print debugging information?"              NO  )
option (DAL_VERBOSE_CONFIGURE  "Verbose output during configuration?"      NO  )

## Handle option: testing mode on/off

if (DAL_ENABLE_TESTING)
  include (CTest)
  enable_testing()
endif (DAL_ENABLE_TESTING)

## Handle option: verbose mode during configuration on/off

if (NOT DAL_VERBOSE_CONFIGURE)
  set (BOOST_FIND_QUIETLY ON)
  set (CASACORE_FIND_QUIETLY ON)
  set (CFITSIO_FIND_QUIETLY ON)
  set (GSL_FIND_QUIETLY ON)
  set (HDF5_FIND_QUIETLY ON)
  set (PYTHON_FIND_QUIETLY ON)
  set (MPATROL_FIND_QUIETLY ON)
  set (MYSQL_FIND_QUIETLY ON)
endif (NOT DAL_VERBOSE_CONFIGURE)

## Handle option: debugging messages on/off

if (DAL_DEBUGGING_MESSAGES)
  add_definitions (-DDEBUGGING_MESSAGES)
endif (DAL_DEBUGGING_MESSAGES)

## ==============================================================================
##
##  External dependencies
##
## ==============================================================================

include (CPack)

set (NO_DEFAULT_PATH ON)

##__________________________________________________________
## system libraries

find_library (libdl dl ${lib_locations})
find_library (libm m ${lib_locations})
find_library (libpthread pthread ${lib_locations})
find_library (libutil util ${lib_locations})
find_library (libz z ${lib_locations})

##__________________________________________________________
## Boost

set (BOOST_pythonOnly NO )

if (NOT HAVE_BOOST)
  include (FindBoost)
endif (NOT HAVE_BOOST)

##__________________________________________________________
## casacore

if (NOT HAVE_CASACORE)
  include (FindCASACORE)
endif (NOT HAVE_CASACORE)

##__________________________________________________________
## CFITSIO

if (NOT HAVE_CFITSIO)
  include (FindCFITSIO)
endif (NOT HAVE_CFITSIO)

##__________________________________________________________
## HDF5

if (NOT HAVE_HDF5)
  include (FindHDF5)
endif (NOT HAVE_HDF5)

## ---------------------------------------------------------
## mpatrol

if (NOT HAVE_MPATROL)
  include (FindMPatrol)
endif (NOT HAVE_MPATROL)

## ---------------------------------------------------------
## MySQL

if (NOT HAVE_MYSQL)
  include (FindMySQL)
endif (NOT HAVE_MYSQL)

##__________________________________________________________
## GNU scientific library (GSL)

if (NOT HAVE_GSL)
  include (FindGSL)
endif (NOT HAVE_GSL)

##__________________________________________________________
## PYTHON (we use custom module in place of FindPythonLibs)

if (NOT HAVE_PYTHON)
  include (FindPython)
endif (NOT HAVE_PYTHON)

## ---------------------------------------------------------
## optional external packages (required for Python bindings)

## We need to be a bit careful with the Boost and Python libraries; appending
## the Boost libraries only makes sense, if there is already a list of Python
## libraries

if (BOOST_LIBRARIES)
  if (PYTHON_LIBRARIES)
    list (APPEND PYTHON_LIBRARIES ${BOOST_LIBRARIES})
  else (PYTHON_LIBRARIES)
    set (PYTHON_LIBRARIES ${BOOST_LIBRARIES})
  endif (PYTHON_LIBRARIES)
endif (BOOST_LIBRARIES)

## Effect of dependencies on build options; if we are unable to locate all the
## required components to generate the Python bindings to the DAL, we have to 
## disable this option - otherwise we won't be able to properly build the test
## programs and applications.

if (NOT BOOST_INCLUDES) 
  set (DAL_PYTHON_BINDINGS OFF)
  message (STATUS "[DAL] Unable to generate Python bindings; missing Boost++ headers!")
endif (NOT BOOST_INCLUDES)

if (NOT PYTHON_INCLUDES) 
  set (DAL_PYTHON_BINDINGS OFF)
  message (STATUS "[DAL] Unable to generate Python bindings; missing Python headers!")
endif (NOT PYTHON_INCLUDES)

if (NOT NUMPY_INCLUDES) 
  set (DAL_PYTHON_BINDINGS OFF)
  message (STATUS "[DAL] Unable to generate Python bindings; missing NumPy headers!")
endif (NOT NUMPY_INCLUDES)

if (NOT NUM_UTIL_INCLUDES) 
  set (DAL_PYTHON_BINDINGS OFF)
  message (STATUS "[DAL] Unable to generate Python bindings; missing num_util headers!")
endif (NOT NUM_UTIL_INCLUDES)

## -----------------------------------------------------------------------------
## Include directories

## header files of the DAL itself

include_directories (
  ${DAL_SOURCE_DIR}/implement
  ${DAL_SOURCE_DIR}/test
  ${DAL_SOURCE_DIR}/apps
  )

## header files of external packages

if (DAL_PYTHON_BINDINGS)

  if (PYTHON_INCLUDES)
    include_directories (${PYTHON_INCLUDES})
  else (PYTHON_INCLUDES)
    message (STATUS "[DAL] Missing Python header files!")
  endif (PYTHON_INCLUDES)

  if (NUMPY_INCLUDES)
    include_directories (${NUMPY_INCLUDES})
  else (NUMPY_INCLUDES)
    message (STATUS "[DAL] Missing numpy header files!")
  endif (NUMPY_INCLUDES)
  
  if (NUM_UTIL_INCLUDES)
    include_directories (${NUM_UTIL_INCLUDES} ${NUM_UTIL_INCLUDES}/num_util)
  else (NUM_UTIL_INCLUDES)
    message (STATUS "[DAL] Missing num_util header files!")
  endif (NUM_UTIL_INCLUDES)
  
endif (DAL_PYTHON_BINDINGS)

if (BOOST_INCLUDES)
  include_directories (${BOOST_INCLUDES} ${BOOST_INCLUDES}/boost)
  add_definitions (-DHAVE_BOOST)
else (BOOST_INCLUDES)
  message (STATUS "[DAL] Missing Boost header files!")
endif (BOOST_INCLUDES)

if (CASACORE_INCLUDES)
  include_directories (${CASACORE_INCLUDES})
  add_definitions (-DHAVE_CASA)
else (CASACORE_INCLUDES)
  message (STATUS "[DAL] Missing casacore header files!")
endif (CASACORE_INCLUDES)

if (CFITSIO_INCLUDES)
  include_directories (${CFITSIO_INCLUDES})
  add_definitions (-DHAVE_CFITSIO)
else (CFITSIO_INCLUDES)
  message (STATUS "[DAL] Missing CFITSIO header files!")
endif (CFITSIO_INCLUDES)

if (HAVE_HDF5)
  include_directories (${HDF5_INCLUDES})
  add_definitions (-DHAVE_HDF5)
  ## check individual header files: H5LT.h
  if (NOT HAVE_HDF5_H5LT_H)
    message (STATUS "[DAL] HDF5 installation incomplete: missing H5LT.h")
  endif (NOT HAVE_HDF5_H5LT_H)
  ## check individual header files: hdf5_hl.h
  if (NOT HAVE_HDF5_HDF5_HL_H)
    message (STATUS "[DAL] HDF5 installation incomplete: missing hdf5_hl.h")
  endif (NOT HAVE_HDF5_HDF5_HL_H)
else (HAVE_HDF5)
  message (STATUS "[DAL] HDF5 installation incomplete!")
endif (HAVE_HDF5)

if (HAVE_MYSQL)
  include_directories (${MYSQL_INCLUDES})
  add_definitions (-DHAVE_MYSQL)
else (HAVE_MYSQL)
  message (STATUS "[DAL] MYSQL installation incomplete!")
endif (HAVE_MYSQL)

if (HAVE_GSL)
  include_directories (${GSL_INCLUDES})
  add_definitions (-DHAVE_GSL)
else (HAVE_GSL)
  message (STATUS "[DAL] GSL installation incomplete!")
endif (HAVE_GSL)

##__________________________________________________________
## Option: Enable checking against memory leaks

if (DAL_MEMORY_CHECKING)
  
  if (APPLE)
    find_library (HAVE_MALLOC_DEBUG MallocDebug ${lib_locations})

    if (HAVE_MALLOC_DEBUG)
      message (STATUS "libMallocDebug = ${HAVE_MALLOC_DEBUG}")
    else (HAVE_MALLOC_DEBUG)
      set (HAVE_MALLOC_DEBUG NO)
    endif (HAVE_MALLOC_DEBUG)
  endif (APPLE) 
  
endif (DAL_MEMORY_CHECKING)

## -----------------------------------------------------------------------------
## Installation

find_path (prefix release_area.txt
  PATHS
  ${DAL_SOURCE_DIR}
  PATH_SUFFIXES
  ../release
  ../../release
  NO_DEFAULT_PATH
  )

if (prefix)
  message (STATUS "Installation area located for package DAL.")
  set (CMAKE_INSTALL_PREFIX ${prefix}) 
endif (prefix)

# skip the full RPATH for the build tree
set (CMAKE_SKIP_BUILD_RPATH TRUE)

# when building, use the install RPATH already
# (so it doesn't need to relink when installing)
set (CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

# the RPATH to be used when installing
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

## =============================================================================
##
##  Subdirectories
##
## =============================================================================

add_subdirectory (implement)

if (DAL_BUILD_TESTS)
  add_subdirectory (test)
endif (DAL_BUILD_TESTS)

if (DAL_BUILD_APPS)
  add_subdirectory (apps)
endif (DAL_BUILD_APPS)

## -----------------------------------------------------------------------------
## Feedback

if (DAL_VERBOSE_CONFIGURE)
  message (STATUS "+============================================================+")
  message (STATUS "| DAL: Summary of configuration settings                     |")
  message (STATUS "+------------------------------------------------------------+")
  message (STATUS " Create Python bindings = ${DAL_PYTHON_BINDINGS}")
  message (STATUS "+------------------------------------------------------------+")
  message (STATUS " HDF5 includes          = ${HDF5_INCLUDES}")
  message (STATUS " HDF5 library           = ${HDF5_LIBRARIES}")
  message (STATUS " Boost includes         = ${BOOST_INCLUDES}")
  message (STATUS " Boost library          = ${BOOST_LIBRARIES}")
  message (STATUS " Python includes        = ${PYTHON_INCLUDES}")
  message (STATUS " NumPy includes         = ${NUMPY_INCLUDES}")
  message (STATUS " num_util includes      = ${NUM_UTIL_INCLUDES}")
  message (STATUS " Python library         = ${PYTHON_LIBRARIES}")
  message (STATUS " casacore               = ${HAVE_CASACORE}")
  message (STATUS "+------------------------------------------------------------+")
endif (DAL_VERBOSE_CONFIGURE)
