##------------------------------------------------------------------------------
## $Id::                                                                       $
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
option (DAL_EXPERIMENTAL_CODE  "Include new expermetal code into build?"   NO  )

## Handle option: testing mode on/off

if (DAL_ENABLE_TESTING)
  include (CTest)
  enable_testing()
endif (DAL_ENABLE_TESTING)

## Handle option: verbose mode during configuration on/off

if (NOT DAL_VERBOSE_CONFIGURE)
  set (BLITZ_FIND_QUIETLY     YES )
  set (BOOST_FIND_QUIETLY     YES )
  set (CASACORE_FIND_QUIETLY  YES )
  set (CFITSIO_FIND_QUIETLY   YES )
  set (GSL_FIND_QUIETLY       YES )
  set (HDF5_FIND_QUIETLY      YES )
  set (LAPACK_FIND_QUIETLY    YES )
  set (NUMPY_FIND_QUIETLY     YES )
  set (NUM_UTIL_FIND_QUIETLY  YES )
  set (PYTHON_FIND_QUIETLY    YES )
  set (MPATROL_FIND_QUIETLY   YES )
  set (MYSQL_FIND_QUIETLY     YES )
  set (WCSLIB_FIND_QUIETLY    YES )
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

set (NO_DEFAULT_PATH YES)

##__________________________________________________________
## System header files

find_path (HAVE_LIBGEN_H     libgen.h      PATHS ${include_locations} )
find_path (HAVE_STDINT_H     stdint.h      PATHS ${include_locations} )
find_path (HAVE_STDLIB_H     stdlib.h      PATHS ${include_locations} )
find_path (HAVE_STRING_H     string.h      PATHS ${include_locations} )
find_path (HAVE_STRINGS_H    strings.h     PATHS ${include_locations} )
find_path (HAVE_SYS_STAT_H   sys/stat.h    PATHS ${include_locations} )
find_path (HAVE_SYS_TIME_H   sys/time.h    PATHS ${include_locations} )
find_path (HAVE_SYS_TYPES_H  sys/types.h   PATHS ${include_locations} )
find_path (HAVE_SYS_UTIME_H  sys/utime.h   PATHS ${include_locations} )

##__________________________________________________________
## System Libraries

find_library (libdl      dl       ${lib_locations})
find_library (libm       m        ${lib_locations})
find_library (libpthread pthread  ${lib_locations})
find_library (libutil    util     ${lib_locations})
find_library (libz       z        ${lib_locations})

##__________________________________________________________
## Type sizes

include (CheckTypeSize)

check_type_size ("short"         SIZEOF_SHORT       )
check_type_size ("int"           SIZEOF_INT         )
check_type_size ("int32_t"       SIZEOF_INT32_T     )
check_type_size ("int64_t"       SIZEOF_INT64_T     )
check_type_size ("float"         SIZEOF_FLOAT       )
check_type_size ("double"        SIZEOF_DOUBLE      )
check_type_size ("long"          SIZEOF_LONG        )
check_type_size ("long int"      SIZEOF_LONG_INT   )
check_type_size ("long long"     SIZEOF_LONG_LONG   )
check_type_size ("uint"          SIZEOF_UINT        )

if (SIZEOF_LONG_LONG)
  add_definitions (-DHAVE_LONG_LONG)
endif (SIZEOF_LONG_LONG)

if (SIZEOF_INT64_T EQUAL SIZEOF_LONG_INT)
  add_definitions (-DINT64_IS_LONG_INT)
endif (SIZEOF_INT64_T EQUAL SIZEOF_LONG_INT)

##__________________________________________________________
## Blitz++

if (NOT HAVE_BLITZ)
  include (FindBlitz)
endif (NOT HAVE_BLITZ)

##__________________________________________________________
## Boost

set (BOOST_pythonOnly FALSE)

if (NOT DAL_PYTHON_BINDINGS)
  set (BOOST_FIND_python FALSE)
endif (NOT DAL_PYTHON_BINDINGS)

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

##__________________________________________________________
## LAPACK

if (NOT HAVE_LAPACK)
  include (FindLAPACK)
endif (NOT HAVE_LAPACK)

##__________________________________________________________
## mpatrol

if (NOT HAVE_MPATROL)
  include (FindMPatrol)
endif (NOT HAVE_MPATROL)

##__________________________________________________________
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
  include (FindNumPy)
  include (FindNumUtil)
endif (NOT HAVE_PYTHON)

##__________________________________________________________
## WCS Library

if (NOT HAVE_WCS)
  include (FindWCS)
endif (NOT HAVE_WCS)

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

## -----------------------------------------------------------------------------
## Include directories

## header files of the DAL itself

include_directories (
  ${DAL_SOURCE_DIR}/implement
  ${DAL_SOURCE_DIR}/test
  ${DAL_SOURCE_DIR}/apps
  )

## header files of external packages

if (HAVE_BLITZ)
  include_directories (${BLITZ_INCLUDES})
  add_definitions (-DHAVE_BLITZ -DBZ_HAVE_NCEG_RESTRICT)
endif (HAVE_BLITZ)

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

if (HAVE_CFITSIO)
  include_directories (${CFITSIO_INCLUDES})
  add_definitions (-DHAVE_CFITSIO)
else (HAVE_CFITSIO)
  message (STATUS "[DAL] Missing or incomplete installation of CFITSIO!")
endif (HAVE_CFITSIO)

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

if (HAVE_MYSQL)
  include_directories (${MYSQL_INCLUDES})
  add_definitions (-DHAVE_MYSQL)
endif (HAVE_MYSQL)

if (HAVE_GSL)
  include_directories (${GSL_INCLUDES})
  add_definitions (-DHAVE_GSL)
endif (HAVE_GSL)

if (HAVE_WCSLIB)
  include_directories (${WCSLIB_INCLUDES})
  add_definitions (-DHAVE_WCSLIB)
endif (HAVE_WCSLIB)

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
  endif (DAL_PYTHON_BINDINGS)
  
endif (DAL_PYTHON_BINDINGS)

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

find_program (tar_executable tar ${bin_locations})
find_program (gzip_executable gzip ${bin_locations})

if (tar_executable AND gzip_executable)

  add_custom_target (tar_archive
    COMMENT "Creating tar-archive for DAL and required packages."
    WORKING_DIRECTORY ${USG_ROOT}
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

message (STATUS "+============================================================+")
message (STATUS "| DAL: Summary of configuration settings                     |")
message (STATUS "+------------------------------------------------------------+")
message (STATUS " Print debugging messages   = ${DAL_DEBUGGING_MESSAGES}")
message (STATUS " Enable Python bindings     = ${DAL_PYTHON_BINDINGS}")
message (STATUS " Enable code using Blitz++  = ${HAVE_BLITZ}")
message (STATUS " Enable code using casacore = ${HAVE_CASACORE}")
message (STATUS " Enable code using CFITSIO  = ${HAVE_CFITSIO}")
message (STATUS " Enable code using GSL      = ${HAVE_GSL}")
message (STATUS " Enable code using MySQL    = ${HAVE_MYSQL}")

if (DAL_VERBOSE_CONFIGURE)
  message (STATUS "+------------------------------------------------------------+")
  message (STATUS " Blitz++ includes         = ${BLITZ_INCLUDES}")
  message (STATUS " Blitz++ library          = ${BLITZ_LIBRARIES}")
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
  message (STATUS " WCSLIB includes          = ${WCSLIB_INCLUDES}")
  message (STATUS " WCSLIB library           = ${WCSLIB_LIBRARIES}")
endif (DAL_VERBOSE_CONFIGURE)

message (STATUS "+------------------------------------------------------------+")
