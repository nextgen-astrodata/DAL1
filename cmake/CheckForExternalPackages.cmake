
##__________________________________________________________
##                                    Standard CMake modules

include (FindMPI)
include (FindOpenMP)

##__________________________________________________________
##                                         LUS CMake modules

find_path (LUS_ROOT devel_common/cmake/CMakeSettings.cmake
  PATHS 
  ${DAL_SOURCE_DIR}
  ${DAL_SOURCE_DIR}/..
  ${DAL_SOURCE_DIR}/../..
  ${DAL_SOURCE_DIR}/../../..
  $ENV{LOFARSOFT}
  )

if (LUS_ROOT)
  foreach (_luscmake
      CMakeSettings
      FindTestDatasets
      FindBoost
      FindGFortran
      FindPython
      FindNumPy
      FindNumUtil
      )
    include (${LUS_ROOT}/devel_common/cmake/${_luscmake}.cmake)
  endforeach (_luscmake)
  ##
  set (CASACORE_FOUND ${HAVE_CASACORE})
endif (LUS_ROOT)

##__________________________________________________________
##                                      Custom CMake modules

foreach (_dalcmake
    FindCasacore
    FindCFITSIO
    FindGSL
    FindHDF5
    FindLAPACK
    FindLOFAR
    FindMySQL
    FindWCSLIB
    )
  include (${DAL_SOURCE_DIR}/cmake/${_dalcmake}.cmake)
endforeach (_dalcmake)

##__________________________________________________________
##                                                Type sizes

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
##                                     External header files

if (HAVE_BOOST)
  include_directories (${BOOST_INCLUDES}/boost)
else (HAVE_BOOST)
  message (STATUS "[DAL] Missing or incomplete installation of Boost!")
endif (HAVE_BOOST)

if (HDF5_FOUND AND HDF5_HDF5_HL_LIBRARY)
  if (HDF5_USE_16_API_DEFAULT)
    add_definitions (-DH5_USE_16_API_DEFAULT=0)
  endif (HDF5_USE_16_API_DEFAULT)
endif (HDF5_FOUND AND HDF5_HDF5_HL_LIBRARY)

foreach (_external
    BOOST
    CASA
    CFITSIO
    GSL
    HDF5
    LOFAR
    MYSQL
    WCSLIB
    )

  if (${_external}_FOUND OR HAVE_${_external})

    ## variable mapping
    set (HAVE_${_external} TRUE)
    
    ## include directories
    include_directories (${${_external}_INCLUDES})

  endif (${_external}_FOUND OR HAVE_${_external})
  
endforeach (_external)

## ==============================================================================
##
##  Configure file
##
##  Any occurrences of #cmakedefine VAR will be replaced with either #define VAR
##  or /* #undef VAR */ depending on the setting of VAR in CMake. Any occurrences
##  of #cmakedefine01 VAR will be replaced with either #define VAR 1 or
##  #define VAR 0 depending on whether VAR evaluates to TRUE or FALSE in CMake
##
## ==============================================================================

configure_file (
  ${DAL_SOURCE_DIR}/cmake/dal_config.h.in
  ${DAL_BINARY_DIR}/dal_config.h
  )

## ==============================================================================
##
##  Feedback
##
## ==============================================================================

message (STATUS "+============================================================+" )
message (STATUS "| DAL: Summary of configuration settings                     |" )
message (STATUS "+------------------------------------------------------------+" )
message (STATUS " Python version                   = ${PYTHON_VERSION}"          )
message (STATUS " Print debugging messages         = ${DAL_DEBUGGING_MESSAGES}"  )
message (STATUS " Enable Dashboard server          = ${DAL_ENABLE_DASHBOARD}"    )
message (STATUS " Enable Python bindings           = ${DAL_PYTHON_BINDINGS}"     )
message (STATUS " Enable code using casacore       = ${CASACORE_FOUND}"          )
message (STATUS " Enable code using CFITSIO        = ${CFITSIO_FOUND}"           )
message (STATUS " Enable code using GSL            = ${GSL_FOUND}"               )
message (STATUS " Enable code using HDF5           = ${HDF5_FOUND}"              )
message (STATUS " .. Version                       = ${HDF5_VERSION}"            )
message (STATUS " .. Parallel I/O                  = ${HDF5_HAVE_PARALLEL_IO}"   )
message (STATUS " .. 1.6 API default               = ${HDF5_USE_16_API_DEFAULT}" )
message (STATUS " Enable code using MPI            = ${MPI_FOUND}"               )
message (STATUS " .. MPI compiler                  = ${MPI_COMPILER}"            )
message (STATUS " .. MPI compile flags             = ${MPI_COMPILE_FLAGS}"       )
message (STATUS " .. MPI linking flags             = ${MPI_LINK_FLAGS}"          )
message (STATUS " Enable code using MySQL          = ${MYSQL_FOUND}"             )
message (STATUS " Enable code using OpenMP         = ${OPENMP_FOUND}"            )
message (STATUS " Enable code using WCSLIB         = ${WCSLIB_FOUND}"            )

if (DAL_VERBOSE_CONFIGURE)
  message (STATUS "+------------------------------------------------------------+")
  message (STATUS " LAPACK library           = ${LAPACK_LIBRARIES}   ")
  message (STATUS " Python includes          = ${PYTHON_INCLUDES}    ")
  message (STATUS " NumPy includes           = ${NUMPY_INCLUDES}     ")
  message (STATUS " num_util includes        = ${NUM_UTIL_INCLUDES}  ")
  message (STATUS " Python library           = ${PYTHON_LIBRARIES}   ")
  message (STATUS " Types sizes:                                     ")
  message (STATUS " .. void*                 = ${CMAKE_SIZEOF_VOID_P}")
  message (STATUS " .. short                 = ${SIZEOF_SHORT}       ")
  message (STATUS " .. int                   = ${SIZEOF_INT}         ")
  message (STATUS " .. float                 = ${SIZEOF_FLOAT}       ")
  message (STATUS " .. double                = ${SIZEOF_DOUBLE}      ")
  message (STATUS " .. long                  = ${SIZEOF_LONG}        ")
  message (STATUS " .. long int              = ${SIZEOF_LONG_INT}    ")
  message (STATUS " .. long long             = ${SIZEOF_LONG_LONG}   ")
  message (STATUS " .. uint                  = ${SIZEOF_UINT}        ")
  message (STATUS " .. int32_t               = ${SIZEOF_INT32_T}     ")
  message (STATUS " .. int64_t               = ${SIZEOF_INT64_T}     ")
  message (STATUS " .. uint32_t              = ${SIZEOF_UINT32_T}    ")
  message (STATUS " .. uint64_t              = ${SIZEOF_UINT64_T}    ")
  message (STATUS "+------------------------------------------------------------+")
endif (DAL_VERBOSE_CONFIGURE)

