##------------------------------------------------------------------------------
## $Id:: CMakeLists.txt 494 2007-08-02 09:39:15Z baehren                       $
##------------------------------------------------------------------------------

## Name of the project
project (DAL)

## Minimum required version of CMake to configure the project
cmake_minimum_required (VERSION 2.4)

## -----------------------------------------------------------------------------
## User defined build/install options

option (DAL_BUILD_TESTS "Build the test programs?" 1)
option (DAL_ENABLE_TESTING "Build the test programs?" 1)
option (DAL_PYTHON_BINDINGS "Create python bindings?" 1)
option (DAL_DEBUGGING_MESSAGES "Print debugging information?" 0)
option (DAL_VERBOSE_CONFIGURE "Verbose output during configuration?" 0)

## Handle option: testing mode on/off

if (DAL_ENABLE_TESTING)
  include (CTest)
  enable_testing()
endif (DAL_ENABLE_TESTING)

## Handle option: verbose mode during configuration on/off

if (NOT DAL_VERBOSE_CONFIGURE)
  set (BOOST_FIND_QUIETLY 1)
  set (CASACORE_FIND_QUIETLY 1)
  set (CFITSIO_FIND_QUIETLY 1)
  set (HDF5_FIND_QUIETLY 1)
  set (PYTHON_FIND_QUIETLY 1)
endif (NOT DAL_VERBOSE_CONFIGURE)

## -----------------------------------------------------------------------------
## External dependencies

include (CPack)

set (lib_locations
  /lib
  /usr/lib
  /usr/local/lib
  /opt/lib
  /sw/lib
  )

## system libraries

find_library (libm m ${lib_locations})
find_library (libdl dl ${lib_locations})
find_library (libutil util ${lib_locations})

## check where to find the CMake scripts

find_path (dal_cmake FindHDF5.cmake FindCASACORE.cmake
  PATHS 
  ${DAL_SOURCE_DIR}
  ${DAL_SOURCE_DIR}/..
  ${DAL_SOURCE_DIR}/../..
  PATH_SUFFIXES
  devel_common/cmake
  )

## check for the external packages

set (NO_DEFAULT_PATH 1)

if (dal_cmake)
  ## casacore
  if (NOT HAVE_CASACORE)
    include (${dal_cmake}/FindCASACORE.cmake)
  endif (NOT HAVE_CASACORE)
  ## CFITSIO
  if (NOT HAVE_CFITSIO)
    include (${dal_cmake}/FindCFITSIO.cmake)
  endif (NOT HAVE_CFITSIO)
  ## HDF5
  if (NOT HAVE_HDF5)
    include (${dal_cmake}/FindHDF5.cmake)
  endif (NOT HAVE_HDF5)
  ## PYTHON (we use custom module in place of FindPythonLibs)
  if (NOT HAVE_PYTHON)
    include (${dal_cmake}/FindPython.cmake)
  endif (NOT HAVE_PYTHON)
  ##----------------------------------------------------------
  ## optional external packages (required for Python bindings)
  if (DAL_PYTHON_BINDINGS)
    ## BOOST
    if (NOT HAVE_BOOST)
      include (${dal_cmake}/FindBoost.cmake)
    endif (NOT HAVE_BOOST)
  endif (DAL_PYTHON_BINDINGS)
elseif (dal_cmake)
  message (SEND_ERROR "Unable to locate additional CMake scripts!")
endif (dal_cmake)

if (BOOST_LIBRARIES)
  list (APPEND PYTHON_LIBRARIES ${BOOST_LIBRARIES})
endif (BOOST_LIBRARIES)

## -----------------------------------------------------------------------------
## Include directories

include_directories (
  ${DAL_SOURCE_DIR}
  ${DAL_SOURCE_DIR}/implement
  ${DAL_SOURCE_DIR}/test
  ${PYTHON_INCLUDES}
  )

if (HAVE_BOOST)
  include_directories (${BOOST_INCLUDES})
else (HAVE_BOOST)
  message (SEND_ERROR "[dal] Missing Boost header files!")
endif (HAVE_BOOST)

if (HAVE_CASACORE)
  include_directories (${CASACORE_INCLUDES})
else (HAVE_CASACORE)
  message (SEND_ERROR "[dal] Missing casacore header files!")
endif (HAVE_CASACORE)

if (HAVE_CFITSIO)
  include_directories (${CFITSIO_INCLUDES})
else (HAVE_CFITSIO)
  message (SEND_ERROR "[dal] Missing CFITSIO header files!")
endif (HAVE_CFITSIO)

if (HAVE_HDF5)
  include_directories (${HDF5_INCLUDES})
else (HAVE_HDF5)
  message (SEND_ERROR "[dal] Missing HDF5 header files!")
endif (HAVE_HDF5)

## -----------------------------------------------------------------------------
## Subdirectories

ADD_SUBDIRECTORY (implement)

if (DAL_BUILD_TESTS)
  ADD_SUBDIRECTORY (test)
endif (DAL_BUILD_TESTS)

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

## -----------------------------------------------------------------------------
## Feedback

if (DAL_VERBOSE_CONFIGURE)
  message (STATUS "HDF5 includes   = ${HDF5_INCLUDES}")
  message (STATUS "HDF5 library    = ${HDF5_LIBRARIES}")
  message (STATUS "Boost includes  = ${BOOST_INCLUDES}")
  message (STATUS "Boost library   = ${BOOST_LIBRARIES}")
  message (STATUS "Python includes = ${PYTHON_INCLUDE_PATH}")
  message (STATUS "Python library  = ${PYTHON_LIBRARIES}")
  message (STATUS "casacore        = ${HAVE_CASACORE}")
endif (DAL_VERBOSE_CONFIGURE)
