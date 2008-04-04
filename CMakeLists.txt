##------------------------------------------------------------------------------
## $Id:: CMakeLists.txt                                                        $
##------------------------------------------------------------------------------

## Name of the project
project (DAL)

## Minimum required version of CMake to configure the project
cmake_minimum_required (VERSION 2.4)

## -----------------------------------------------------------------------------
## User defined build/install options

option (DAL_BUILD_TESTS "Build the test programs?" 1)
option (DAL_BUILD_APPS "Build the applications?" 1)
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
  set (BOOST_FIND_QUIETLY ON)
  set (CASACORE_FIND_QUIETLY ON)
  set (CFITSIO_FIND_QUIETLY ON)
  set (HDF5_FIND_QUIETLY ON)
  set (PYTHON_FIND_QUIETLY ON)
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

find_library (libdl dl ${lib_locations})
find_library (libm m ${lib_locations})
find_library (libpthread pthread ${lib_locations})
find_library (libutil util ${lib_locations})
find_library (libz z ${lib_locations})

## check where to find the CMake scripts

find_path (dal_cmake CMakeSettings.cmake
  PATHS 
  ${DAL_SOURCE_DIR}
  ${DAL_SOURCE_DIR}/..
  ${DAL_SOURCE_DIR}/../..
  PATH_SUFFIXES
  devel_common/cmake
  )

if (dal_cmake)
  list (APPEND CMAKE_MODULE_PATH
 ${dal_cmake})
else (dal_cmake)
  message (FATAL_ERROR "Unable to locate additional CMake scripts!")
endif (dal_cmake)

## check for the external packages

set (NO_DEFAULT_PATH ON)

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
    set (BOOST_pythonOnly ON)
    set (BOOST_test_exec_monitor OFF)
    if (NOT HAVE_BOOST)
      include (${dal_cmake}/FindBoost.cmake)
    endif (NOT HAVE_BOOST)
  endif (DAL_PYTHON_BINDINGS)
elseif (dal_cmake)
  message (SEND_ERROR "Unable to locate additional CMake scripts!")
endif (dal_cmake)

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

if (HDF5_INCLUDES)
  include_directories (${HDF5_INCLUDES})
else (HDF5_INCLUDES)
  message (STATUS "[DAL] Missing HDF5 header files!")
endif (HDF5_INCLUDES)

## -----------------------------------------------------------------------------
## Subdirectories

add_subdirectory (implement)

if (DAL_BUILD_TESTS)
  add_subdirectory (test)
endif (DAL_BUILD_TESTS)

if (DAL_BUILD_APPS)
  add_subdirectory (apps)
endif (DAL_BUILD_APPS)

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

## -----------------------------------------------------------------------------
## Feedback

if (DAL_VERBOSE_CONFIGURE)
  message (STATUS "+============================================================+")
  message (STATUS "| DAL: Summary of configuration settings                     |")
  message (STATUS "+------------------------------------------------------------+")
  message (STATUS " HDF5 includes     = ${HDF5_INCLUDES}")
  message (STATUS " HDF5 library      = ${HDF5_LIBRARIES}")
  message (STATUS " Boost includes    = ${BOOST_INCLUDES}")
  message (STATUS " Boost library     = ${BOOST_LIBRARIES}")
  message (STATUS " Python includes   = ${PYTHON_INCLUDES}")
  message (STATUS " NumPy includes    = ${NUMPY_INCLUDES}")
  message (STATUS " num_util includes = ${NUM_UTIL_INCLUDES}")
  message (STATUS " Python library    = ${PYTHON_LIBRARIES}")
  message (STATUS " casacore          = ${HAVE_CASACORE}")
  message (STATUS "+------------------------------------------------------------+")
endif (DAL_VERBOSE_CONFIGURE)
