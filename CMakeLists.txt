##------------------------------------------------------------------------------
## $Id::                                                                       $
##------------------------------------------------------------------------------

## Name of the project
project (DAL)

## Minimum required version of CMake to configure the project
cmake_minimum_required (VERSION 2.8)

## ==============================================================================
##
##  Options
##
## ==============================================================================

option (DAL_BUILD_TESTS        "Build the test programs?"                     YES )
option (DAL_BUILD_APPS         "Build the applications?"                      YES )
option (DAL_ENABLE_TESTING     "Build the test programs?"                     YES )
option (DAL_ENABLE_DASHBOARD   "Send a build report to the Dashboard server?" YES )
option (DAL_COMPILER_WARNINGS  "Enable standard set of compiler warnings?"    ON  )
option (DAL_HDF5_MACROS        "HDF5 compatibility macros version"            YES )
option (DAL_PYTHON_BINDINGS    "Create python bindings?"                      YES )
option (DAL_DEBUGGING_MESSAGES "Print debugging information?"                 NO  )
option (DAL_VERBOSE_CONFIGURE  "Verbose output during configuration?"         NO  )

##__________________________________________________________
## Handle option: testing mode

if (DAL_ENABLE_DASHBOARD OR DAL_ENABLE_TESTING)
  ## Include CTest module
  include (CTest)
  enable_testing()
  ## Control parameters for running the tests
  set (DART_TESTING_TIMEOUT 500)
endif (DAL_ENABLE_DASHBOARD OR DAL_ENABLE_TESTING)

##__________________________________________________________
## Handle option: verbose mode during configuration?

if (NOT DAL_VERBOSE_CONFIGURE)
  set (BOOST_FIND_QUIETLY     YES )
  set (CASACORE_FIND_QUIETLY  YES )
  set (CFITSIO_FIND_QUIETLY   YES )
  set (GFORTRAN_FIND_QUIETLY  YES )
  set (GSL_FIND_QUIETLY       YES )
  set (HDF5_FIND_QUIETLY      YES )
  set (LAPACK_FIND_QUIETLY    YES )
  set (LOFAR_FIND_QUIETLY     YES )
  set (MPI_FIND_QUIETLY       YES )
  set (NUMPY_FIND_QUIETLY     YES )
  set (NUM_UTIL_FIND_QUIETLY  YES )
  set (PYTHON_FIND_QUIETLY    YES )
  set (MPI_FIND_QUIETLY       YES )
  set (MYSQL_FIND_QUIETLY     YES )
  set (WCSLIB_FIND_QUIETLY    YES )
endif (NOT DAL_VERBOSE_CONFIGURE)

##__________________________________________________________
## Handle option: debugging messages?

if (DAL_DEBUGGING_MESSAGES)
  add_definitions (-DDEBUGGING_MESSAGES)
endif (DAL_DEBUGGING_MESSAGES)

##__________________________________________________________
## Handle option: Enable standard set of compiler warnings?

if (DAL_COMPILER_WARNINGS)
  set(CMAKE_CXX_FLAGS "-W -g -Wall")
endif (DAL_COMPILER_WARNINGS)

add_definitions (
  -DMPICH_IGNORE_CXX_SEEK
  -D_FILE_OFFSET_BITS=64
  -D_LARGEFILE64_SOURCE
  -D_LARGEFILE_SOURCE
  -fPIC
  )

##__________________________________________________________
## Handle option: Compatibility macros to version 2

if (DAL_HDF5_MACROS)
  add_definitions (-DHDF5_MACROS_VERS2)
endif (DAL_HDF5_MACROS)

## ==============================================================================
##
##  External dependencies
##
## ==============================================================================

include (CPack)
include (${DAL_SOURCE_DIR}/cmake/CheckForExternalPackages.cmake)

##__________________________________________________________
## PYTHON (we use custom module in place of FindPythonLibs)

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

if (NOT NUMPY_INCLUDES) 
  set (DAL_PYTHON_BINDINGS FALSE)
  message (STATUS "[DAL] Unable to generate Python bindings; missing NumPy headers!")
endif (NOT NUMPY_INCLUDES)

if (NOT NUM_UTIL_INCLUDES) 
  set (DAL_PYTHON_BINDINGS FALSE)
  message (STATUS "[DAL] Unable to generate Python bindings; missing num_util headers!")
endif (NOT NUM_UTIL_INCLUDES)

## ==============================================================================
##
##  Include directories
##
## ==============================================================================

##__________________________________________________________
## Header files of the DAL itself

include_directories (
  ${DAL_SOURCE_DIR}/implement
  ${DAL_SOURCE_DIR}/implement/core
  ${DAL_SOURCE_DIR}/implement/coordinates
  ${DAL_SOURCE_DIR}/implement/data_common
  ${DAL_SOURCE_DIR}/implement/data_hl
  ${DAL_SOURCE_DIR}/test
  ${DAL_SOURCE_DIR}/test/hdf5
  ${DAL_SOURCE_DIR}/apps
  )

##__________________________________________________________
## Option: Generation of Python bindings (pydal)

if (DAL_PYTHON_BINDINGS)
  
  ## Try to compile test program
  
  file (WRITE ${PROJECT_BINARY_DIR}/CMakeTmp/CMakeLists.txt
    "project (TestForPythonBindings)
     cmake_minimum_required(VERSION 2.6)
     include_directories (${PYTHON_INCLUDES})
     include_directories (${BOOST_INCLUDES})
     include_directories (${NUMPY_INCLUDES})
     include_directories (${NUM_UTIL_INCLUDES} ${NUM_UTIL_INCLUDES}/num_util)
     ADD_EXECUTABLE(TestForPythonBindings ${CMAKE_MODULE_PATH}/TestForPythonBindings.cc)
     TARGET_LINK_LIBRARIES(TestForPythonBindings ${PYTHON_LIBRARIES} ${NUM_UTIL_LIBRARIES})"
    )
  
  try_compile (DAL_PYTHON_BINDINGS
    ${PROJECT_BINARY_DIR}/CMakeTmp
    ${PROJECT_BINARY_DIR}/CMakeTmp
    TestForPythonBindings
    )

  ## Enable include directories of test compile succeeded
  
  if (DAL_PYTHON_BINDINGS)
    include_directories (${PYTHON_INCLUDES})
    include_directories (${NUMPY_INCLUDES})
    include_directories (${NUM_UTIL_INCLUDES} ${NUM_UTIL_INCLUDES}/num_util)
  else (DAL_PYTHON_BINDINGS)
      message (STATUS "[DAL] Test compilation of Python bindings failed; disabling")
  endif (DAL_PYTHON_BINDINGS)
  
endif (DAL_PYTHON_BINDINGS)

## =============================================================================
##
##  List of external libraries against which to link; even though we really
##  need the external libraries listed here, we should be a bit careful when
##  dealing with the related CMake variables - check first if the variable is
##  set before adding it to the list of link libraries.
##
## =============================================================================

set (dal_link_libraries "")

if (HAVE_LIBZ)
  list (APPEND dal_link_libraries ${HAVE_LIBZ})
endif (HAVE_LIBZ)

if (CASA_LIBRARIES)
  list (APPEND dal_link_libraries ${CASA_LIBRARIES})
endif (CASA_LIBRARIES)

if (HDF5_LIBRARIES)
  list (APPEND dal_link_libraries ${HDF5_LIBRARIES})
else (HDF5_LIBRARIES)
  message (FATAL_ERROR "[DAL] Missing HDF5 library file(s)!")
endif (HDF5_LIBRARIES)

if (LAPACK_LIBRARIES)
  list (APPEND dal_link_libraries ${LAPACK_LIBRARIES})
endif (LAPACK_LIBRARIES)

if (CFITSIO_LIBRARIES)
  list (APPEND dal_link_libraries ${CFITSIO_LIBRARIES})
endif (CFITSIO_LIBRARIES)

if (WCSLIB_LIBRARIES)
  list (APPEND dal_link_libraries ${WCSLIB_LIBRARIES})
endif (WCSLIB_LIBRARIES)

if (HAVE_LIBPTHREAD)
  list (APPEND dal_link_libraries ${HAVE_LIBPTHREAD})
endif (HAVE_LIBPTHREAD)

if (HAVE_LIBDL)
  list (APPEND dal_link_libraries ${HAVE_LIBDL})
endif (HAVE_LIBDL)

if (HAVE_LIBM)
  list (APPEND dal_link_libraries ${HAVE_LIBM})
endif (HAVE_LIBM)

if (HAVE_LIBUTIL)
  list (APPEND dal_link_libraries ${HAVE_LIBUTIL})
endif (HAVE_LIBUTIL)

if (HAVE_MYSQL)
  list (APPEND dal_link_libraries ${MYSQL_LIBRARIES})
endif (HAVE_MYSQL)

## Parallel IO usage requires MPI to be Linked and Included

if (MPI_FOUND AND HDF5_HAVE_PARALLEL_IO)
  ## compiler flags
  if (MPI_COMPILE_FLAGS)
    add_definitions (${MPI_COMPILE_FLAGS})
  endif (MPI_COMPILE_FLAGS)
  ## linker flags
  if (MPI_LINK_FLAGS)
    set (CMAKE_EXE_LINKER_FLAGS ${MPI_LINK_FLAGS} ${CMAKE_EXE_LINKER_FLAGS})
  endif (MPI_LINK_FLAGS)
  ## link libraries
  list (APPEND dal_link_libraries ${MPI_LIBRARY})
  if (MPI_EXTRA_LIBRARY)
    list (APPEND dal_link_libraries ${MPI_EXTRA_LIBRARY})
  endif (MPI_EXTRA_LIBRARY)
endif (MPI_FOUND AND HDF5_HAVE_PARALLEL_IO)

## =============================================================================
##
##  Installation
##
## =============================================================================

# skip the full RPATH for the build tree
set (CMAKE_SKIP_BUILD_RPATH YES)

# when building, use the install RPATH already
# (so it doesn't need to relink when installing)
set (CMAKE_BUILD_WITH_INSTALL_RPATH YES)

# the RPATH to be used when installing
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH YES)

## =============================================================================
##
##  Project sub-directories
##
## =============================================================================

add_subdirectory (data)
add_subdirectory (implement)

if (DAL_BUILD_TESTS)
  add_subdirectory (test)
endif (DAL_BUILD_TESTS)

if (DAL_BUILD_APPS)
  add_subdirectory (apps)
endif (DAL_BUILD_APPS)
