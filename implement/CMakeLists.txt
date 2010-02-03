##------------------------------------------------------------------------------
## $Id::                                                                       $
##------------------------------------------------------------------------------

## -----------------------------------------------------------------------------
## TODO:
## 
## - translate the following build instructions to CMake settings:
##    g++ -c *.cpp -ftemplate-depth-100 -fPIC -I/usr/include/python2.4 -I.
##    g++ *.o -lboost_python -lcfitsio -lhdf5 -lhdf5_hl -lhdf5_cpp -lz -lm -o dal.so -shared

## ==============================================================================
##
##  Compiler flags
##
## ==============================================================================

if (DAL_COMPILER_WARNINGS)
  set(CMAKE_CXX_FLAGS "-W -g -Wall")
endif (DAL_COMPILER_WARNINGS)

message(STATUS "cflags: ${CMAKE_CXX_FLAGS}")

add_definitions (
  -DMPICH_IGNORE_CXX_SEEK
  -D_FILE_OFFSET_BITS=64
  -D_LARGEFILE64_SOURCE
  -D_LARGEFILE_SOURCE
  -fPIC
  -ftemplate-depth-100
  )

if (HAVE_CASA OR HAVE_CASACORE)
  add_definitions (
    -DHAVE_CASA
    # -- compiler flags utilized in the source code --
    -DAIPS_STDLIB
    -DAIPS_AUTO_STL
    )
  IF (APPLE)
    ## definition common to apple architectures
    add_definitions (
      -DAIPS_DARWIN
      )
  ELSE (APPLE)
    ADD_DEFINITIONS (-DAIPS_LINUX)
  ENDIF (APPLE)
  ## Platform test 32/64 bit ------------------------------
  set (CMAKE_SYSTEM_64BIT 0)
  if (NOT CMAKE_SYSTEM_PROCESSOR MATCHES i386)
    if (NOT CMAKE_SYSTEM_PROCESSOR MATCHES i686)
      if (NOT CMAKE_SYSTEM_PROCESSOR MATCHES powerpc)
        set (CMAKE_SYSTEM_64BIT 1)
        add_definitions (-DAIPS_64B)
      endif (NOT CMAKE_SYSTEM_PROCESSOR MATCHES powerpc)
    endif (NOT CMAKE_SYSTEM_PROCESSOR MATCHES i686)
  endif (NOT CMAKE_SYSTEM_PROCESSOR MATCHES i386)
  if (APPLE)
    if (NOT CMAKE_SYSTEM_PROCESSOR MATCHES powerpc)
      set (CMAKE_SYSTEM_64BIT 1)
      add_definitions (-DAIPS_64B)
    endif (NOT CMAKE_SYSTEM_PROCESSOR MATCHES powerpc)
  endif (APPLE)
endif (HAVE_CASA OR HAVE_CASACORE)

## ==============================================================================
##
##  External libraries to link against
##
## ==============================================================================


## ==============================================================================
##
##  Build the library, "libdal"
##
## ==============================================================================

##__________________________________________________________
## Define the list of modules

set (dal_modules
  core
  coordinates
  data_common
  data_hl
  )

##__________________________________________________________
## Lists of source and header files

file (GLOB dal_headers *.h)

set (dal_sources
  dalFITS.cc
  )

if ( HAVE_CASACORE )
  list ( APPEND dal_sources dalLopesEvent.cc )
endif ( HAVE_CASACORE )

##__________________________________________________________
## Process the contents of the modules directories

foreach (dal_module ${dal_modules})
  ## Add include paths
  include_directories (${DAL_SOURCE_DIR}/implement/${dal_module})
  ## list of source files for this module
  FILE (GLOB dal_${dal_module}_sources ${dal_module}/*.cpp ${dal_module}/*.cc)
  ## list of header files for this module
  FILE (GLOB dal_${dal_module}_headers ${dal_module}/*.h)
  ## expand the list of source files
  list (APPEND dal_sources ${dal_${dal_module}_sources})
  list (APPEND dal_headers ${dal_${dal_module}_headers})
endforeach (dal_module)

##____________________________________________________________________
## Build the library

add_library(dal STATIC ${dal_sources})

target_link_libraries(dal ${dal_link_libraries})

## ------------------------------------------------------------------------------
## Build the library for the Python bindings, "libpydal"

if (DAL_PYTHON_BINDINGS)
  ## compiler instructions
  remove_definitions (-dynamiclib -install_name)
  ## Source files entering the library
  add_library(pydal MODULE
    ${dal_sources}
    pywrapper.cc
    )
  include_directories (
    ${NUM_UTIL_INCLUDES}
    )
  ## linker instructions
  target_link_libraries(pydal
    ${dal_link_libraries}
    ${PYTHON_LIBRARIES}
    ${BOOST_LIBRARIES}
    ${NUM_UTIL_LIBRARIES}
    ${CFITSIO_LIBRARIES}
    ${WCSLIB_LIBRARIES}
    ${LAPACK_LIBRARIES}
    ${HAVE_LIBDL}
    ${HAVE_LIBPTHREAD}
    ${HAVE_LIBUTIL}
    )
  ## additional target properties
  if (APPLE)
    set_target_properties (pydal
      PROPERTIES
      PREFIX ""
      SUFFIX .so
      COMPILE_FLAGS "-DPYTHON -fpermissive"
      LINK_FLAGS "-fPIC -flat_namespace"
      )
  else (APPLE)
    set_target_properties (pydal
      PROPERTIES
      PREFIX ""
      SUFFIX .so
      COMPILE_FLAGS -DPYTHON
      LINK_FLAGS "-fPIC -shared"
      )
  endif (APPLE)
endif (DAL_PYTHON_BINDINGS)

## ------------------------------------------------------------------------------
## Installation

## installation of libraries

install (
  TARGETS dal
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  )

if (DAL_PYTHON_BINDINGS)
  install (
    TARGETS pydal
    LIBRARY DESTINATION lib/python
    )
endif (DAL_PYTHON_BINDINGS)

## installation of header files

install (FILES ${dal_headers}
  DESTINATION include/dal
  )
