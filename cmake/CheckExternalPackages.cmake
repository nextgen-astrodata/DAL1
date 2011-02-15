
##__________________________________________________________
##                                    Standard CMake modules

include (FindMPI)
include (FindOpenMP)

##__________________________________________________________
##                                      Custom CMake modules

foreach (_dalcmake
    Boost
    Casacore
    CFITSIO
    GFortran
    GSL
    HDF5
    LAPACK
    LOFAR
    MySQL
    NumPy
    NumUtil
    Python
    WCSLIB
    )

  message (STATUS "Checking for package ${_dalcmake}")

  ## Generate uppercase version of package name
  string (TOUPPER ${_dalcmake} _var)
  ## Provide potential root directory of package
  set (${_var}_ROOT_DIR ${CMAKE_INSTALL_PREFIX})
  ## Include CMake find script
  include (${DAL_SOURCE_DIR}/cmake/Find${_dalcmake}.cmake)

  if (${_var}_FOUND OR HAVE_${_var})

    ## variable mapping
    set (HAVE_${_var} TRUE)
    
    ## include directories
    include_directories (${${_var}_INCLUDES})

    message (STATUS "Checking for package ${_dalcmake} - Success")

  endif (${_var}_FOUND OR HAVE_${_var})

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

## ==============================================================================
##
##  Python bindings
##
## ==============================================================================

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

if (NOT NUMUTIL_INCLUDES) 
  set (DAL_PYTHON_BINDINGS FALSE)
  message (STATUS "[DAL] Unable to generate Python bindings; missing num_util headers!")
endif (NOT NUMUTIL_INCLUDES)

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

install (FILES ${DAL_BINARY_DIR}/dal_config.h
  DESTINATION include/dal
  )

## ==============================================================================
##
##  Feedback
##
## ==============================================================================

message (STATUS "+============================================================+"   )
message (STATUS "| DAL: Summary of configuration settings                     |"   )
message (STATUS "+------------------------------------------------------------+"   )
message (STATUS " CMAKE_INSTALL_PREFIX             = ${CMAKE_INSTALL_PREFIX}"      )
message (STATUS " Build and enable test programs   = ${DAL_ENABLE_TESTING}"        )
message (STATUS " Print debugging messages         = ${DAL_DEBUGGING_MESSAGES}"    )
message (STATUS " Enable code using Boost++        = ${BOOST_FOUND}"               )
message (STATUS " .. Library version               = ${BOOST_VERSION}"             )
message (STATUS " .. Include directory             = ${BOOST_INCLUDES}"            )
message (STATUS " Enable code using casacore       = ${CASACORE_FOUND}"            )
message (STATUS " Enable code using CFITSIO        = ${CFITSIO_FOUND}"             )
message (STATUS " Enable code using GSL            = ${GSL_FOUND}"                 )
message (STATUS " Enable code using HDF5           = ${HDF5_FOUND}"                )
message (STATUS " .. Library version               = ${HDF5_VERSION}"              )
message (STATUS " .. Include directory             = ${HDF5_INCLUDES}"             )
message (STATUS " .. Parallel I/O                  = ${HDF5_HAVE_PARALLEL_IO}"     )
message (STATUS " .. 1.6 API default               = ${HDF5_USE_16_API_DEFAULT}"   )
message (STATUS " Enable code using MPI            = ${MPI_FOUND}"                 )
message (STATUS " .. MPI compiler                  = ${MPI_COMPILER}"              )
message (STATUS " .. MPI compile flags             = ${MPI_COMPILE_FLAGS}"         )
message (STATUS " .. MPI linking flags             = ${MPI_LINK_FLAGS}"            )
message (STATUS " Enable code using MySQL          = ${MYSQL_FOUND}"               )
message (STATUS " .. Library version               = ${MYSQL_VERSION}"             )
message (STATUS " .. Port number                   = ${MYSQL_PORT}"                )
message (STATUS " .. libmysql                      = ${MYSQL_MYSQL_LIBRARY}"       )
message (STATUS " .. libmysqlclient                = ${MYSQL_MYSQLCLIENT_LIBRARY}" )
message (STATUS " .. libmysqlservices              = ${MYSQL_MYSQLSERVICES_LIBRARY}" )
message (STATUS " Enable Python bindings           = ${DAL_PYTHON_BINDINGS}"       )
message (STATUS " .. Python version                = ${PYTHON_VERSION}"            )
message (STATUS " .. Python API version            = ${PYTHON_API_VERSION}"        )
message (STATUS " .. Python NumUtils package       = ${NUMUTIL_FOUND}"             )
message (STATUS " Enable code using OpenMP         = ${OPENMP_FOUND}"              )
message (STATUS " Enable code using WCSLIB         = ${WCSLIB_FOUND}"              )
message (STATUS "+------------------------------------------------------------+"   )

if (DAL_VERBOSE_CONFIGURE)
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

