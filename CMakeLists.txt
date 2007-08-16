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
option (DAL_DEBUGGING_MESSAGES "Print debugging information?" 0)
option (DAL_VERBOSE_CONFIGURE "Verbose output during configuration?" 0)

## Handle option: testing mode on/off

if (DAL_ENABLE_TESTING)
  include (CTest)
  enable_testing()
endif (DAL_ENABLE_TESTING)

## Handle option: verbose mode during configuration on/off

if (NOT DAL_VERBOSE_CONFIGURE)
  set (BLITZ_FIND_QUIETLY 1)
  set (BOOST_FIND_QUIETLY 1)
  set (CASACORE_FIND_QUIETLY 1)
  set (CFITSIO_FIND_QUIETLY 1)
  set (HDF5_FIND_QUIETLY 1)
  set (SWIG_FIND_QUIETLY 1)
endif (NOT DAL_VERBOSE_CONFIGURE)

## -----------------------------------------------------------------------------
## External dependencies

include (CPack)

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

if (dal_cmake)
  ## BLITZ++
  if (NOT HAVE_BLITZ)
    include (${dal_cmake}/FindBlitz.cmake)
  endif (NOT HAVE_BLITZ)
  ## BOOST
  if (NOT HAVE_BOOST)
    include (${dal_cmake}/FindBoost.cmake)
  endif (NOT HAVE_BOOST)
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
  ## SWIG
  if (NOT HAVE_SWIG)
    include (${dal_cmake}/FindSWIG.cmake)
  endif (NOT HAVE_SWIG)
elseif (dal_cmake)
  message (SEND_ERROR "Unable to locate additional CMake scripts!")
endif (dal_cmake)

include (FindPythonLibs)

if (BOOST_LIBRARIES)
  list (APPEND PYTHON_LIBRARIES ${BOOST_LIBRARIES})
endif (BOOST_LIBRARIES)

## -----------------------------------------------------------------------------
## Location of header files

include_directories (
  ${DAL_SOURCE_DIR}
  ${DAL_SOURCE_DIR}/implement
  ${DAL_SOURCE_DIR}/test
  ${BLITZ_INCLUDES}
  ${BOOST_INCLUDES}
  ${CASA_INCLUDES}
  ${CFITSIO_INCLUDES}
  ${HDF5_INCLUDES}
  ${PYTHON_INCLUDE_PATH}
  )

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
