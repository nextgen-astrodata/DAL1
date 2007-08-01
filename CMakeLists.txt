
## Project name

project (DAL)

## subdirectories to process

subdirs (
  implement
  test
)

## ------------------------------------------------------------------------------
## External dependencies

## check where to find the CMake scripts

find_path (dal_cmake template_FindXX.cmake 
  PATHS 
  ${DAL_SOURCE_DIR}
  ${DAL_SOURCE_DIR}/..
  ${DAL_SOURCE_DIR}/../..
  PATH_SUFFIXES
  devel_common/cmake
  )

if (dal_cmake)
  ## HDF5
  if (NOT HAVE_HDF5)
    include (${dal_cmake}/FindHDF5.cmake)
  endif (NOT HAVE_HDF5)
  ## CFITSIO
  if (NOT HAVE_CFITSIO)
    include (${dal_cmake}/FindCFITSIO.cmake)
  endif (NOT HAVE_CFITSIO)
  ## BLITZ++
  if (NOT HAVE_BLITZ)
    include (${dal_cmake}/FindBlitz.cmake)
  endif (NOT HAVE_BLITZ)
  ## BOOST
  if (NOT HAVE_BOOST)
    include (${dal_cmake}/FindBoost.cmake)
  endif (NOT HAVE_BOOST)
  ## SWIG
  if (NOT HAVE_SWIG)
    include (${dal_cmake}/FindSWIG.cmake)
  endif (NOT HAVE_SWIG)
  ## casacore
  if (NOT HAVE_CASACORE)
    include (${dal_cmake}/FindCASACORE.cmake)
  endif (NOT HAVE_CASACORE)
elseif (dal_cmake)
  message (SEND_ERROR "Unable to locate additional CMake scripts!")
endif (dal_cmake)

include (FindPythonLibs)

if (libboost_python)
  list (APPEND PYTHON_LIBRARIES ${libboost_python})
endif (libboost_python)

message (STATUS "Boost includes        = ${BOOST_INCLUDES}")
message (STATUS "Boost library         = ${BOOST_LIBRARIES}")
message (STATUS "Python includes       = ${PYTHON_INCLUDE_PATH}")
message (STATUS "Python library        = ${PYTHON_LIBRARIES}")

## ------------------------------------------------------------------------------
## Location of header files

include_directories(
  .
  ${DAL_SOURCE_DIR}/implement
  ${DAL_SOURCE_DIR}/test
  ${HDF5_INCLUDES}
  ${CFITSIO_INCLUDES}
  ${BLITZ_INCLUDES}
  ${BOOST_INCLUDES}
  ${PYTHON_INCLUDE_PATH}
  )

## ------------------------------------------------------------------------------
## Installation

include (CPack)

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
