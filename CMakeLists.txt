##------------------------------------------------------------------------------
## $Id::                                                                       $
##------------------------------------------------------------------------------

## Name of the project
project (DAL)

## Minimum required version of CMake to configure the project
cmake_minimum_required (VERSION 2.8)

## Custom CMake scripts
include (${PROJECT_SOURCE_DIR}/CMakeMacros.cmake)

## ==============================================================================
##
##  CMake configuration settings
##
## ==============================================================================

find_path (LUS_ROOT devel_common/cmake/CMakeSettings.cmake
  PATHS 
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/..
  ${PROJECT_SOURCE_DIR}/../..
  ${PROJECT_SOURCE_DIR}/../../..
  $ENV{LOFARSOFT}
  )

if (LUS_ROOT)
  include (${LUS_ROOT}/devel_common/cmake/CMakeSettings.cmake)
else (LUS_ROOT)
  message (FATAL_ERROR "Unable to locate additional CMake scripts!")
endif (LUS_ROOT)

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
  include (FindTestDatasets)
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

if (HAVE_OPNEMP)
  add_definitions (-DHAVE_OPNEMP)
endif (HAVE_OPNEMP)

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
  -fpermissive
  -ftemplate-depth-100
  )

## See comment on http://gcc.gnu.org/bugzilla/show_bug.cgi?id=41645
if (APPLE)
  add_definitions (
    -D_GLIBCXX_FULLY_DYNAMIC_STRING
    )
endif (APPLE)

## ==============================================================================
##
##  External dependencies
##
## ==============================================================================

include (CPack)
include (IncludeFindModule)

set (NO_DEFAULT_PATH YES)

set (dal_cmake_modules
  Boost
  CASACORE
  GFortran
  HDF5
  LAPACK
  LOFAR
  MPI
  MySQL
  )

include_find_module (${dal_cmake_modules} ${DAL_VERBOSE_CONFIGURE})

include (cmake/FindCFITSIO.cmake)
include (cmake/FindGSL.cmake)
include (cmake/FindWCSLIB.cmake)

##__________________________________________________________
## Type sizes

if (DAL_VERBOSE_CONFIGURE)
  message (STATUS "Size of void*     = ${CMAKE_SIZEOF_VOID_P}")
  message (STATUS "Size of short     = ${SIZEOF_SHORT}")
  message (STATUS "Size of int       = ${SIZEOF_INT}")
  message (STATUS "Size of float     = ${SIZEOF_FLOAT}")
  message (STATUS "Size of double    = ${SIZEOF_DOUBLE}")
  message (STATUS "Size of long      = ${SIZEOF_LONG}")
  message (STATUS "Size of long int  = ${SIZEOF_LONG_INT}")
  message (STATUS "Size of long long = ${SIZEOF_LONG_LONG}")
  message (STATUS "Size of uint      = ${SIZEOF_UINT}")
  message (STATUS "Size of int32_t   = ${SIZEOF_INT32_T}")
  message (STATUS "Size of int64_t   = ${SIZEOF_INT64_T}")
  message (STATUS "Size of uint32_t  = ${SIZEOF_UINT32_T}")
  message (STATUS "Size of uint64_t  = ${SIZEOF_UINT64_T}")
endif (DAL_VERBOSE_CONFIGURE)

if (SIZEOF_LONG_LONG)
  add_definitions (-DHAVE_LONG_LONG)
endif (SIZEOF_LONG_LONG)

if (CMAKE_SIZEOF_VOID_P)
  if (${CMAKE_SIZEOF_VOID_P} EQUAL 8)
    message (STATUS "Adding compiler flag -DWORDSIZE_IS_64")
    add_definitions (-DWORDSIZE_IS_64)
  endif (${CMAKE_SIZEOF_VOID_P} EQUAL 8)
endif (CMAKE_SIZEOF_VOID_P)

##__________________________________________________________
## PYTHON (we use custom module in place of FindPythonLibs)

if (NOT HAVE_PYTHON)
  include (FindPython)
  include (FindNumPy)
  include (FindNumUtil)
endif (NOT HAVE_PYTHON)

## ---------------------------------------------------------
## optional external packages (required for Python bindings)

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
## Header files of external packages

if (HAVE_BOOST)
  include_directories (${BOOST_INCLUDES} ${BOOST_INCLUDES}/boost)
  add_definitions (-DHAVE_BOOST)
else (HAVE_BOOST)
  message (STATUS "[DAL] Missing or incomplete installation of Boost!")
endif (HAVE_BOOST)

if (HAVE_CASACORE)
  include_directories (${CASACORE_INCLUDES})
  add_definitions (-DHAVE_CASA)
else (HAVE_CASACORE)
  message (STATUS "[DAL] Missing or incomplete installation of casacore!")
endif (HAVE_CASACORE)

if (CFITSIO_FOUND)
  include_directories (${CFITSIO_INCLUDES})
  add_definitions (-DHAVE_CFITSIO)
else (CFITSIO_FOUND)
  message (STATUS "[DAL] Missing or incomplete installation of CFITSIO!")
endif (CFITSIO_FOUND)

if (HAVE_HDF5)
  include_directories (${HDF5_INCLUDES})
  add_definitions (-DHAVE_HDF5)
  ## check individual header files: hdf5_hl.h
  if (NOT HAVE_HDF5_HDF5_HL_H)
    message (STATUS "[DAL] HDF5 installation incomplete: missing hdf5_hl.h")
  endif (NOT HAVE_HDF5_HDF5_HL_H)
else (HAVE_HDF5)
  message (STATUS "[DAL] HDF5 installation incomplete!")
endif (HAVE_HDF5)

if (LOFAR_FOUND)
  include_directories (${LOFAR_INCLUDES})
  add_definitions (-DHAVE_LOFAR)
endif (LOFAR_FOUND)

if (HAVE_MYSQL)
  include_directories (${MYSQL_INCLUDES})
  add_definitions (-DHAVE_MYSQL)
endif (HAVE_MYSQL)

if (GSL_FOUND)
  include_directories (${GSL_INCLUDES})
  add_definitions (-DHAVE_GSL)
else (GSL_FOUND)
  message (STATUS "[DAL] Disabling support for using GSL!")
endif (GSL_FOUND)

if (WCSLIB_FOUND)
  include_directories (${WCSLIB_INCLUDES})
  add_definitions (-DHAVE_WCSLIB)
else (WCSLIB_FOUND)
  message (STATUS "[DAL] Disabling support for using WCSLIB!")
endif (WCSLIB_FOUND)

##__________________________________________________________
## Option: Generation of Python bindings (pydal)

if (DAL_PYTHON_BINDINGS)
  
  ## Try to compile test program
  
  file (WRITE ${PROJECT_BINARY_DIR}/CMakeTmp/CMakeLists.txt
    "project (TestForPythonBindings)
     cmake_minimum_required(VERSION 2.5)
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

##__________________________________________________________
## Option: Compatibility macros to version 2

if (DAL_HDF5_MACROS)
  add_definitions (-DHDF5_MACROS_VERS2)
endif (DAL_HDF5_MACROS)

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

if (HAVE_LIBDL)
  list (APPEND dal_link_libraries ${HAVE_LIBDL})
endif (HAVE_LIBDL)

if (HAVE_LIBM)
  list (APPEND dal_link_libraries ${HAVE_LIBM})
endif (HAVE_LIBM)

if (HAVE_LIBPTHREAD)
  list (APPEND dal_link_libraries ${HAVE_LIBPTHREAD})
endif (HAVE_LIBPTHREAD)

if (HAVE_LIBUTIL)
  list (APPEND dal_link_libraries ${HAVE_LIBUTIL})
endif (HAVE_LIBUTIL)

if (HAVE_MYSQL)
  list (APPEND dal_link_libraries ${MYSQL_LIBRARIES})
endif (HAVE_MYSQL)

## Parallel IO usage requires MPI to be Linked and Included

if (MPI_FOUND AND HDF5_IS_PARALLEL)
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
endif (MPI_FOUND AND HDF5_IS_PARALLEL)

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
##  Creation of tar-archive
##
## =============================================================================

find_program (tar_executable tar   ${bin_locations} )
find_program (gzip_executable gzip ${bin_locations} )

if (tar_executable AND gzip_executable)

  add_custom_target (tar_archive
    COMMENT "Creating tar-archive for DAL and required packages."
    WORKING_DIRECTORY ${LUS_ROOT}
    COMMAND ${tar_executable} -cvf dal.tar --exclude .svn CMakeLists.txt
    COMMAND ${tar_executable} -rvf dal.tar --exclude .svn devel_common
    COMMAND ${tar_executable} -rvf dal.tar --exclude .svn release/release_area.txt
    COMMAND ${tar_executable} -rvf dal.tar --exclude .svn build/build.sh
    COMMAND ${tar_executable} -rvf dal.tar --exclude .svn external/cmake
    COMMAND ${tar_executable} -rvf dal.tar --exclude .svn external/flex
    COMMAND ${tar_executable} -rvf dal.tar --exclude .svn external/bison
    COMMAND ${tar_executable} -rvf dal.tar --exclude .svn external/wcslib
    COMMAND ${tar_executable} -rvf dal.tar --exclude .svn external/cfitsio
    COMMAND ${tar_executable} -rvf dal.tar --exclude .svn external/hdf5
    COMMAND ${tar_executable} -rvf dal.tar --exclude .svn external/casacore
    COMMAND ${tar_executable} -rvf dal.tar --exclude .svn external/plplot
    COMMAND ${tar_executable} -rvf dal.tar --exclude .svn external/boost
    COMMAND ${tar_executable} -rvf dal.tar --exclude .svn external/python
    COMMAND ${tar_executable} -rvf dal.tar --exclude .svn src/DAL
    COMMAND ${tar_executable} -rvf dal.tar --exclude .svn src/contrib/data_inspection
    COMMAND ${gzip_executable} dal.tar
    COMMAND mv dal.tar.gz ${DAL_BINARY_DIR}/dal.tgz
    )
 
endif (tar_executable AND gzip_executable)

## =============================================================================
##
##  Project sub-directories
##
## =============================================================================

add_subdirectory (scripts)
add_subdirectory (implement)

if (DAL_BUILD_TESTS)
  add_subdirectory (test)
endif (DAL_BUILD_TESTS)

if (DAL_BUILD_APPS)
  add_subdirectory (apps)
endif (DAL_BUILD_APPS)

## =============================================================================
##
##  Feedback
##
## =============================================================================

message (STATUS "+============================================================+")
message (STATUS "| DAL: Summary of configuration settings                     |")
message (STATUS "+------------------------------------------------------------+")
message (STATUS " Python version                   = ${PYTHON_VERSION}"         )
message (STATUS " Print debugging messages         = ${DAL_DEBUGGING_MESSAGES}" )
message (STATUS " Enable Dashboard server          = ${DAL_ENABLE_DASHBOARD}"   )
message (STATUS " Enable Python bindings           = ${DAL_PYTHON_BINDINGS}"    )
message (STATUS " Enable code using casacore       = ${HAVE_CASACORE}"          )
message (STATUS " Enable code using CFITSIO        = ${CFITSIO_FOUND}"          )
message (STATUS " Enable code using GSL            = ${GSL_FOUND}"              )
message (STATUS " Enable code using HDF5           = ${HAVE_HDF5}"              )
message (STATUS " .. Version                       = ${HDF5_VERSION}"           )
message (STATUS " .. Parallel I/O                  = ${HDF5_IS_PARALLEL}"       )
message (STATUS " .. Compatibility macros          = ${DAL_HDF5_MACROS}"        )
message (STATUS " Enable code using MPI            = ${MPI_FOUND}"              )
message (STATUS " .. MPI compiler                  = ${MPI_COMPILER}"           )
message (STATUS " .. MPI compile flags             = ${MPI_COMPILE_FLAGS}"      )
message (STATUS " .. MPI linking flags             = ${MPI_LINK_FLAGS}"         )
message (STATUS " Enable code using MySQL          = ${HAVE_MYSQL}"             )
message (STATUS " Enable code using OpenMP         = ${HAVE_OPENMP}"            )
message (STATUS " Enable code using WCSLIB         = ${WCSLIB_FOUND}"           )

if (DAL_VERBOSE_CONFIGURE)
  message (STATUS "+------------------------------------------------------------+")
  message (STATUS " Boost includes           = ${BOOST_INCLUDES}")
  message (STATUS " Boost library            = ${BOOST_LIBRARIES}")
  message (STATUS " HDF5 includes            = ${HDF5_INCLUDES}")
  message (STATUS " HDF5 library             = ${HDF5_LIBRARIES}")
  message (STATUS " LAPACK library           = ${LAPACK_LIBRARIES}")
  message (STATUS " Python includes          = ${PYTHON_INCLUDES}")
  message (STATUS " NumPy includes           = ${NUMPY_INCLUDES}")
  message (STATUS " num_util includes        = ${NUM_UTIL_INCLUDES}")
  message (STATUS " Python library           = ${PYTHON_LIBRARIES}")
  message (STATUS " MySQL includes           = ${MYSQL_INCLUDES}")
  message (STATUS " MySQL libraries          = ${MYSQL_LIBRARIES}")
endif (DAL_VERBOSE_CONFIGURE)

message (STATUS "+------------------------------------------------------------+")
