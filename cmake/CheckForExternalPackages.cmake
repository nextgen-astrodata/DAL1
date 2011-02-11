
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
      FindCASACORE
      FindGFortran
      FindPython
      FindNumPy
      FindNumUtil
      )
    include (${LUS_ROOT}/devel_common/cmake/${_luscmake}.cmake)
  endforeach (_luscmake)
endif (LUS_ROOT)

##__________________________________________________________
##                                      Custom CMake modules

foreach (_dalcmake
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

if (HDF5_FOUND AND HDF5_HDF5_HL_LIBRARY)
  ## include directories
  include_directories (${HDF5_INCLUDES})
  ## compiler flags
  add_definitions (-DHAVE_HDF5)
  if (HDF5_USE_16_API_DEFAULT)
    add_definitions (-DH5_USE_16_API_DEFAULT=0)
  endif (HDF5_USE_16_API_DEFAULT)
else (HDF5_FOUND AND HDF5_HDF5_HL_LIBRARY)
  message (STATUS "[DAL] HDF5 installation incomplete!")
endif (HDF5_FOUND AND HDF5_HDF5_HL_LIBRARY)

if (LOFAR_FOUND)
  include_directories (${LOFAR_INCLUDES})
  add_definitions (-DHAVE_LOFAR)
endif (LOFAR_FOUND)

if (MYSQL_FOUND)
  include_directories (${MYSQL_INCLUDES})
  add_definitions (-DHAVE_MYSQL)
endif (MYSQL_FOUND)

if (GSL_FOUND)
  include_directories (${GSL_INCLUDES})
  add_definitions (-DHAVE_GSL)
else (GSL_FOUND)
  message (STATUS "[DAL] Disabling support for using GSL!")
endif (GSL_FOUND)

if (HAVE_OPNEMP)
  add_definitions (-DHAVE_OPNEMP)
endif (HAVE_OPNEMP)

if (WCSLIB_FOUND)
  include_directories (${WCSLIB_INCLUDES})
  add_definitions (-DHAVE_WCSLIB)
else (WCSLIB_FOUND)
  message (STATUS "[DAL] Disabling support for using WCSLIB!")
endif (WCSLIB_FOUND)

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
message (STATUS " Enable code using casacore       = ${HAVE_CASACORE}"           )
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

