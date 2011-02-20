
## Name of the project
project (DAL)

## Project release version
set (DAL_VERSION_MAJOR 0)
set (DAL_VERSION_MINOR 1)
set (DAL_VERSION_PATCH 0)
set (DAL_VERSION "${DAL_VERSION_MAJOR}.${DAL_VERSION_MINOR}.${DAL_VERSION_PATCH}")

## Minimum required version of CMake to configure the project
cmake_minimum_required (VERSION 2.8)

## ==============================================================================
##
##  Options
##
## ==============================================================================

option (DAL_BUILD_APPS         "Build the applications?"                    YES )
option (DAL_ENABLE_TESTING     "Build and enable test programs?"            YES )
option (DAL_COMPILER_WARNINGS  "Enable standard set of compiler warnings?"  ON  )
option (DAL_PYTHON_BINDINGS    "Create python bindings?"                    YES )
option (DAL_DEBUGGING_MESSAGES "Print debugging information?"               NO  )
option (DAL_VERBOSE_CONFIGURE  "Verbose output during configuration?"       NO  )

##__________________________________________________________
## Handle option: testing mode

if (DAL_ENABLE_TESTING)
  ## Include CTest module
  include (CTest)
  enable_testing()
  ## Control parameters for running the tests
  set (DART_TESTING_TIMEOUT 500)
endif (DAL_ENABLE_TESTING)

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
  set (NUMUTIL_FIND_QUIETLY   YES )
  set (PYTHON_FIND_QUIETLY    YES )
  set (MPI_FIND_QUIETLY       YES )
  set (MYSQL_FIND_QUIETLY     YES )
  set (WCSLIB_FIND_QUIETLY    YES )
endif (NOT DAL_VERBOSE_CONFIGURE)

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

if (DAL_DEBUGGING_MESSAGES)
  set (DEBUGGING_MESSAGES 1)
else (DAL_DEBUGGING_MESSAGES)
  set (DEBUGGING_MESSAGES 0)
endif (DAL_DEBUGGING_MESSAGES)

## ==============================================================================
##
##  External dependencies
##
## ==============================================================================

include (${DAL_SOURCE_DIR}/cmake/CheckExternalPackages.cmake)

## ==============================================================================
##
##  Include directories
##
## ==============================================================================

##__________________________________________________________
## Header files of the DAL itself

include_directories (
  ${DAL_BINARY_DIR}
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
     include_directories (${NUMUTIL_INCLUDES} ${NUMUTIL_INCLUDES}/num_util)
     ADD_EXECUTABLE(TestForPythonBindings ${DAL_SOURCE_DIR}/cmake/TestPythonBindings.cc)
     TARGET_LINK_LIBRARIES(TestForPythonBindings ${PYTHON_LIBRARIES} ${NUMUTIL_LIBRARIES})"
    )
  
  try_compile (DAL_PYTHON_BINDINGS
    ${PROJECT_BINARY_DIR}/CMakeTmp
    ${PROJECT_BINARY_DIR}/CMakeTmp
    TestForPythonBindings
    )

  ## Enable include directories of test compile succeeded
  
  if (DAL_PYTHON_BINDINGS)
    include_directories (${PYTHON_INCLUDES})
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

if (Z_LIBRARY)
  list (APPEND dal_link_libraries ${Z_LIBRARY})
endif (Z_LIBRARY)

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

if (PTHREAD_LIBRARY)
  list (APPEND dal_link_libraries ${PTHREAD_LIBRARY})
endif (PTHREAD_LIBRARY)

if (MYSQL_FOUND)
  list (APPEND dal_link_libraries ${MYSQL_LIBRARIES})
endif (MYSQL_FOUND)

if (UNIX)
  if (NOT APPLE)
    if (UTIL_LIBRARY)
      list (APPEND dal_link_libraries ${UTIL_LIBRARY})
    endif (UTIL_LIBRARY)
    
    if (DL_LIBRARY)
      list (APPEND dal_link_libraries ${DL_LIBRARY})
    endif (DL_LIBRARY)
    
    if (M_LIBRARY)
      list (APPEND dal_link_libraries ${M_LIBRARY})
    endif (M_LIBRARY)
  endif (NOT APPLE)
endif (UNIX)

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

## Clean up the list of link libraries

list (REMOVE_DUPLICATES dal_link_libraries)

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

add_subdirectory (implement)
add_subdirectory (data)

if (DOXYGEN_FOUND)
  add_subdirectory (doc)
endif (DOXYGEN_FOUND)

if (DAL_ENABLE_TESTING)
  add_subdirectory (test)
endif (DAL_ENABLE_TESTING)

if (DAL_BUILD_APPS)
  add_subdirectory (apps)
endif (DAL_BUILD_APPS)


## =============================================================================
##
##  Packaging
##
## =============================================================================

## Package description
SET (CPACK_PACKAGE_NAME                "DAL"                           )
SET (CPACK_PACKAGE_VENDOR              "DAL developers team"           )
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "The Data Access Library (DAL)" )
SET (CPACK_PACKAGE_DESCRIPTION_FILE    "${PROJECT_SOURCE_DIR}/README"  )

# Full version number
set (VERSION                     ${DAL_VERSION}       )
set (CPACK_PACKAGE_VERSION_MAJOR ${DAL_VERSION_MAJOR} )
set (CPACK_PACKAGE_VERSION_MINOR ${DAL_VERSION_MINOR} )
set (CPACK_PACKAGE_VERSION_PATCH ${DAL_VERSION_PATCH} )

## Package generator settings
SET (CPACK_SYSTEM_NAME              "source" )
SET (CPACK_GENERATOR                "TGZ"    )
SET (CPACK_SOURCE_GENERATOR         "TGZ"    )
SET (CPACK_SOURCE_PACKAGE_FILE_NAME "dal-${DAL_VERSION}")

# Load packaging tool
include (CPack)
