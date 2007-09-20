##------------------------------------------------------------------------------
## $Id:: CMakeLists.txt 600 2007-08-22 11:40:47Z baehren                       $
##------------------------------------------------------------------------------

## -----------------------------------------------------------------------------
## TODO:
## 
## - translate the following build instructions to CMake settings:
##    g++ -c *.cpp -ftemplate-depth-100 -fPIC -I/usr/include/python2.4 -I.
##    g++ *.o -lboost_python -lcfitsio -lhdf5 -lhdf5_hl -lhdf5_cpp -lz -lm -o dal.so -shared

## -----------------------------------------------------------------------------
## Compiler flags

set(CMAKE_CXX_FLAGS "-W -Wall")

message(STATUS "cflags: ${CMAKE_CXX_FLAGS}")

add_definitions (
  -DMPICH_IGNORE_CXX_SEEK
#  -D_BSD_SOURCE
#  -D_POSIX_SOURCE 
  -D_FILE_OFFSET_BITS=64
  -D_LARGEFILE64_SOURCE
  -D_LARGEFILE_SOURCE
  -fPIC
  -ftemplate-depth-100
  )

if (APPLE)
  add_definitions (
    -flat_namespace
    -bundle
    )
endif (APPLE)

if (HAVE_CASA OR HAVE_CASACORE)
  add_definitions (
    -DWITH_CASA
    # -- compiler flags utilized in the source code --
    -DAIPS_STDLIB
    -DAIPS_AUTO_STL
    -DAIPS_NO_LEA_MALLOC
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

## ------------------------------------------------------------------------------
## Build the library, "libdal"

## [1] list of source files

set (dal_sources
  dalArray.cpp
  dalAttribute.cpp
  dalBaseTypes.cpp 
  dalColumn.cpp
  dalData.cpp
  dalDataset.cpp
  dalGroup.cpp
  dalRow.cpp
  dalTable.cpp
#  l_member.cpp
  dalLopesEvent.cpp
  )

if (DAL_PYTHON_BINDINGS)
  list (APPEND dal_sources pywrapper.cpp)
endif (DAL_PYTHON_BINDINGS)

## [2] List of external libraries against which to link

set (dal_link_libraries
  hdf5
  hdf5_hl
  z
  m
  ${CFITSIO_LIBRARIES}
)

if (DAL_PYTHON_BINDINGS)
  list (APPEND dal_link_libraries ${BOOST_LIBRARIES} ${PYTHON_LIBRARIES})
  if (HAVE_CASA OR HAVE_CASACORE)
    list (APPEND dal_link_libraries dal ${CASA_LIBRARIES} ${libm})
  endif (HAVE_CASA OR HAVE_CASACORE)
endif (DAL_PYTHON_BINDINGS)

## [3] Build the library

add_library(dal STATIC ${dal_sources})
target_link_libraries(dal
  ${dal_link_libraries}
  )

## ------------------------------------------------------------------------------
## Build the library for the Python bindings, "libpydal"

if (DAL_PYTHON_BINDINGS)
  ## compiler instructions
  add_definitions (-DPYTHON -fPIC)
  add_library(pydal SHARED ${dal_sources})
  ## linker instructions
  target_link_libraries(pydal
    ${dal_link_libraries}
    )
  ## additional target properties
  set_target_properties (pydal
    PROPERTIES
    SUFFIX .so)
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
  LIBRARY DESTINATION lib
  )
endif (DAL_PYTHON_BINDINGS)

## installation of header files

file (GLOB dal_headers *.h)

install (FILES ${dal_headers}
  DESTINATION include/dal
  )
