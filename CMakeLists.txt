
## ==============================================================================
##
##  CMake project settings
##
## ==============================================================================

## Name of the project
project (DAL)

## Project release version
set (DAL_VERSION_MAJOR 0)
set (DAL_VERSION_MINOR 1)
set (DAL_VERSION_PATCH 7)
set (DAL_VERSION "${DAL_VERSION_MAJOR}.${DAL_VERSION_MINOR}.${DAL_VERSION_PATCH}")

## Minimum required version of CMake to configure the project
cmake_minimum_required (VERSION 2.8.3)

## ==============================================================================
##
##  Options
##
## ==============================================================================

option (DOCUMENTATION_ONLY     "Configure for building documentation only?"  NO  )
option (DAL_WITH_DOCUMENTATION "Generate documentation for the DAL?"         YES )
option (DAL_SHARED_LIBRARY     "Build DAL as shared library?"                NO  )
option (DAL_ENABLE_TESTING     "Build and enable test programs?"             YES )
option (DAL_COMPILER_WARNINGS  "Enable standard set of compiler warnings?"   YES )
option (DAL_PYTHON_BINDINGS    "Create python bindings?"                     YES )
option (DAL_DEBUGGING_MESSAGES "Print debugging information?"                NO  )
option (DAL_VERBOSE_CONFIGURE  "Verbose output during configuration?"        NO  )
option (DAL_WITH_MYSQL         "Build with support for MySQL database?"      NO  )

find_program (HOSTNAME_CMD NAMES hostname)
exec_program (${HOSTNAME_CMD} ARGS OUTPUT_VARIABLE CMAKE_SYSTEM_HOSTNAME)

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
## Handle option: Enable standard set of compiler warnings?

if (DAL_COMPILER_WARNINGS)
  set(CMAKE_CXX_FLAGS "-W -Wall")
endif (DAL_COMPILER_WARNINGS)

add_definitions (
  -DMPICH_IGNORE_CXX_SEEK
  -D_FILE_OFFSET_BITS=64
  -D_LARGEFILE64_SOURCE
  -D_LARGEFILE_SOURCE
  -fPIC
  )

## =============================================================================
##
##  - Installation settings
##  - External dependencies
##
## ==============================================================================

include (${DAL_SOURCE_DIR}/cmake/InstallationSettings.cmake)
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
  ${DAL_SOURCE_DIR}/src
  ${DAL_SOURCE_DIR}/test
  ${DAL_SOURCE_DIR}/test/hdf5
  ${DAL_SOURCE_DIR}/apps
  )

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

if (LAPACK_LIBRARIES)
  list (APPEND dal_link_libraries ${LAPACK_LIBRARIES})
endif (LAPACK_LIBRARIES)

if (HDF5_LIBRARIES)
  list (APPEND dal_link_libraries ${HDF5_LIBRARIES})
endif (HDF5_LIBRARIES)

if (CFITSIO_LIBRARIES)
  list (APPEND dal_link_libraries ${CFITSIO_LIBRARIES})
endif (CFITSIO_LIBRARIES)

if (WCSLIB_LIBRARIES)
  list (APPEND dal_link_libraries ${WCSLIB_LIBRARIES})
endif (WCSLIB_LIBRARIES)

if (BOOST_LIBRARIES)
  ## libboost_program_options
  if (Boost_PROGRAM_OPTIONS_LIBRARY)
    list (APPEND dal_link_libraries ${Boost_PROGRAM_OPTIONS_LIBRARY})
  endif (Boost_PROGRAM_OPTIONS_LIBRARY)
  ## libboost_thread
  if (Boost_THREAD_LIBRARY)
    list (APPEND dal_link_libraries ${Boost_THREAD_LIBRARY})
  endif (Boost_THREAD_LIBRARY)
endif (BOOST_LIBRARIES)

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
#  if (MPI_LINK_FLAGS)
#    set (CMAKE_EXE_LINKER_FLAGS ${MPI_LINK_FLAGS} ${CMAKE_EXE_LINKER_FLAGS})
#  endif (MPI_LINK_FLAGS)
  ## link libraries
  if (MPI_LIBRARIES)
    list (APPEND dal_link_libraries ${MPI_LIBRARIES})
  endif (MPI_LIBRARIES)
endif (MPI_FOUND AND HDF5_HAVE_PARALLEL_IO)

## Clean up the list of link libraries

if (dal_link_libraries)
  list (REMOVE_DUPLICATES dal_link_libraries)
endif (dal_link_libraries)

## =============================================================================
##
##  Project sub-directories
##
## =============================================================================

## Option: generate documentation?

if (DAL_WITH_DOCUMENTATION)
  add_subdirectory (doc)
endif (DAL_WITH_DOCUMENTATION)

## Option: only generate documentation?

if (NOT DOCUMENTATION_ONLY)
  
  add_subdirectory (implement)
  add_subdirectory (src)
  add_subdirectory (data)
  add_subdirectory (apps)
  
  if (DAL_ENABLE_TESTING)
    add_subdirectory (test)
  endif (DAL_ENABLE_TESTING)
  
endif (NOT DOCUMENTATION_ONLY)

## =============================================================================
##
##  Packaging
##
## =============================================================================

## Package description
set (CPACK_PACKAGE_NAME                "DAL"                           )
set (CPACK_PACKAGE_VENDOR              "DAL developers team"           )
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "The Data Access Library (DAL)" )
set (CPACK_PACKAGE_DESCRIPTION_FILE    "${PROJECT_SOURCE_DIR}/README"  )
set (CPACK_SOURCE_IGNORE_FILES         "${CMAKE_BINARY_DIR}/*;${PROJECT_SOURCE_DIR}/.git")

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

## Load packaging tool
include (CPack)

## =============================================================================
##
##  Preparing release version
##
## =============================================================================

if (GIT_FOUND AND GIT_REMOTE_ORIGIN)
  
  add_custom_target (release
    COMMAND ${GIT_EXECUTABLE} add CMakeLists.txt
    COMMAND ${GIT_EXECUTABLE} commit -m "DAL release, version ${DAL_VERSION}"
    COMMAND ${GIT_EXECUTABLE} tag v${DAL_VERSION} -m "DAL release, version ${DAL_VERSION}"
    COMMAND ${GIT_EXECUTABLE} push ${GIT_REMOTE_ORIGIN} --tags
    COMMAND ${GIT_EXECUTABLE} push ${GIT_REMOTE_ORIGIN} release
    COMMAND make package_source
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMENT "... preparing DAL release, version ${DAL_VERSION} ..."
    )
  
endif (GIT_FOUND AND GIT_REMOTE_ORIGIN)

## ==============================================================================
##
##  Summary of configuration settings
##
## ==============================================================================

message (STATUS "=============================================================="   )
message (STATUS "  DAL: Summary of configuration settings                      "   )
message (STATUS "+------------------------------------------------------------+"   )
message (STATUS " System configuration:"                                           )
message (STATUS " .. Hostname                     = ${CMAKE_SYSTEM_HOSTNAME}"      )
message (STATUS " .. Processor type               = ${CMAKE_SYSTEM_PROCESSOR}"     )
message (STATUS " .. CMake version                = ${CMAKE_VERSION}"              )
message (STATUS " .. System name                  = ${CMAKE_SYSTEM}"               )
message (STATUS " .. C++ compiler                 = ${CMAKE_CXX_COMPILER}"         )
message (STATUS " .. C compiler                   = ${CMAKE_C_COMPILER}"           )
message (STATUS " Version control configuration:"                                  )
message (STATUS " .. Git version                  = ${GIT_VERSION}"                )
message (STATUS " .. ID for the remote origin     = ${GIT_REMOTE_ORIGIN}"          )
message (STATUS " Installation configuration:"                                     )
message (STATUS " .. DAL_VERSION                  = ${DAL_VERSION}"                )
message (STATUS " .. DAL_INSTALL_PREFIX           = ${DAL_INSTALL_PREFIX}"         )
message (STATUS " .. DAL_INSTALL_BINDIR           = ${DAL_INSTALL_BINDIR}"         )
message (STATUS " .. DAL_INSTALL_INCLUDEDIR       = ${DAL_INSTALL_INCLUDEDIR}"     )
message (STATUS " .. DAL_INSTALL_LIBDIR           = ${DAL_INSTALL_LIBDIR}"         )
message (STATUS " .. DAL_INSTALL_DATAROOTDIR      = ${DAL_INSTALL_DATAROOTDIR}"    )
message (STATUS " .. DAL_INSTALL_DOCDIR           = ${DAL_INSTALL_DOCDIR}"         )
message (STATUS " Build shared library?           = ${DAL_SHARED_LIBRARY}"         )
message (STATUS " Build and enable test programs  = ${DAL_ENABLE_TESTING}"         )
message (STATUS " Print debugging messages        = ${DAL_DEBUGGING_MESSAGES}"     )
message (STATUS " Build documentation only?       = ${DOCUMENTATION_ONLY}"         )
if (LATEX_FOUND OR DAL_VERBOSE_CONFIGURE)
message (STATUS " .. LaTeX compiler               = ${LATEX_EXECUTABLE}"           )
message (STATUS " .. dvips converter              = ${DVIPS_EXECUTABLE}"           )
message (STATUS " .. PDFLaTeX compiler            = ${PDFLATEX_EXECUTABLE}"        )
message (STATUS " .. LaTeX package fncychap       = ${LATEX_FNCYCHAP_PACKAGE}"     )
message (STATUS " .. LaTeX package hyperref       = ${LATEX_HYPERREF_PACKAGE}"     )
message (STATUS " .. LaTeX package listings       = ${LATEX_LISTINGS_PACKAGE}"     )
message (STATUS " .. LaTeX package makeidx        = ${LATEX_MAKEIDX_PACKAGE}"      )
endif (LATEX_FOUND OR DAL_VERBOSE_CONFIGURE)

if (NOT DOCUMENTATION_ONLY)

message (STATUS " Enable code using Boost++       = ${BOOST_FOUND}"               )
message (STATUS " .. Library version              = ${BOOST_VERSION}"             )
message (STATUS " .. Include directory            = ${BOOST_INCLUDES}"            )
message (STATUS " .. date_time library            = ${Boost_DATE_TIME_LIBRARY}")
message (STATUS " .. python library               = ${Boost_PYTHON_LIBRARY}")
message (STATUS " .. program_options library      = ${Boost_PROGRAM_OPTIONS_LIBRARY}")
message (STATUS " .. thread library               = ${Boost_THREAD_LIBRARY}"      )
message (STATUS " .. Thread support enabled?      = ${BOOST_WITH_THREAD}"         )

message (STATUS " Enable code using casacore      = ${CASACORE_FOUND}"            )
message (STATUS " .. Include directory            = ${CASACORE_INCLUDES}"         )

message (STATUS " Enable code using CFITSIO       = ${CFITSIO_FOUND}"             )
if (CFITSIO_FOUND OR DAL_VERBOSE_CONFIGURE)
message (STATUS " .. Library version              = ${CFITSIO_VERSION}"           )
message (STATUS " .. nof. I/O buffers to create   = ${CFITSIO_NIOBUF}"            )
message (STATUS " .. Size per I/O buffer (byte)   = ${CFITSIO_IOBUFLEN}"          )
message (STATUS " .. Maximal length of filename   = ${CFITSIO_FLEN_FILENAME}"     )
message (STATUS " .. Maximal length of a keyword  = ${CFITSIO_FLEN_KEYWORD}"      )
endif (CFITSIO_FOUND OR DAL_VERBOSE_CONFIGURE)

message (STATUS " Enable code using GSL           = ${GSL_FOUND}"                 )

message (STATUS " Enable code using HDF5          = ${HDF5_FOUND}"                )
if (HDF5_FOUND OR DAL_VERBOSE_CONFIGURE)
message (STATUS " .. Library version              = ${HDF5_VERSION}"              )
message (STATUS " .. Include directory            = ${HDF5_INCLUDES}"             )
message (STATUS " .. Parallel I/O                 = ${HDF5_HAVE_PARALLEL_IO}"     )
message (STATUS " .. 1.6 API default              = ${HDF5_USE_16_API_DEFAULT}"   )
message (STATUS " .. HDF5 C compiler              = ${HDF5_C_COMPILER}"           )
message (STATUS " .. HDF5 C++ compiler            = ${HDF5_CXX_COMPILER}"         )
endif (HDF5_FOUND OR DAL_VERBOSE_CONFIGURE)

message (STATUS " Enable code using MPI           = ${MPI_FOUND}"                 )
if (MPI_FOUND OR DAL_VERBOSE_CONFIGURE)
message (STATUS " .. MPI compiler                 = ${MPI_COMPILER}"              )
message (STATUS " .. MPI compile flags            = ${MPI_COMPILE_FLAGS}"         )
message (STATUS " .. MPI linking flags            = ${MPI_LINK_FLAGS}"            )
message (STATUS " .. MPI library                  = ${MPI_LIBRARY}"               )
endif (MPI_FOUND OR DAL_VERBOSE_CONFIGURE)

message (STATUS " Enable code using MySQL         = ${DAL_WITH_MYSQL}"            )
if (MYSQL_FOUND OR DAL_VERBOSE_CONFIGURE)
message (STATUS " .. Library version              = ${MYSQL_VERSION}"             )
message (STATUS " .. Port number                  = ${MYSQL_PORT}"                )
message (STATUS " .. libmysql                     = ${MYSQL_MYSQL_LIBRARY}"       )
message (STATUS " .. libmysqlclient               = ${MYSQL_MYSQLCLIENT_LIBRARY}" )
message (STATUS " .. libmysqlservices             = ${MYSQL_MYSQLSERVICES_LIBRARY}" )
endif (MYSQL_FOUND OR DAL_VERBOSE_CONFIGURE)

message (STATUS " Enable Python bindings          = ${DAL_PYTHON_BINDINGS}"      )
message (STATUS " .. Python version               = ${PYTHON_VERSION}"           )
message (STATUS " .. Python API version           = ${PYTHON_API_VERSION}"       )
message (STATUS " .. Include directory            = ${PYTHON_INCLUDES}"          )
message (STATUS " .. Python interpreter           = ${PYTHON_EXECUTABLE}"        )
message (STATUS " .. Python library               = ${PYTHON_LIBRARIES}"         )
message (STATUS " .. Site-packages directory      = ${PYTHON_SITE_PACKAGES_DIR}" )
message (STATUS " .. NumPy include directory      = ${NUMPY_INCLUDES}"           )
message (STATUS " .. NumPy library                = ${NUMPY_LIBRARIES}"          )
message (STATUS " Enable code using OpenMP        = ${OPENMP_FOUND}"             )
message (STATUS " Enable code using WCSLIB        = ${WCSLIB_FOUND}"             )
message (STATUS " Types sizes:                                     ")
message (STATUS " .. void*                        = ${CMAKE_SIZEOF_VOID_P}")

endif (NOT DOCUMENTATION_ONLY)

message (STATUS "==============================================================" )
