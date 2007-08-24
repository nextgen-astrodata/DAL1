##------------------------------------------------------------------------------
## $Id:: CMakeLists.txt 494 2007-08-02 09:39:15Z baehren                       $
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

#if(NOT CMAKE_BUILD_TYPE)
#    set(CMAKE_BUILD_TYPE "Debug")
#endif(NOT CMAKE_BUILD_TYPE)

add_definitions (
  -DMPICH_IGNORE_CXX_SEEK
  -D_BSD_SOURCE
  -D_POSIX_SOURCE 
  -D_FILE_OFFSET_BITS=64
  -D_LARGEFILE64_SOURCE
  -D_LARGEFILE_SOURCE
  -fPIC
  -ftemplate-depth-100
  )

if (DAL_PYTHON_BINDINGS)
  add_definitions (
    -DPYTHON
  )
endif (DAL_PYTHON_BINDINGS)

#link_directories(
#  ${HDF5_LIBRARIES}
#  ${BLITZ_LIBRARIES}
#  )

## ------------------------------------------------------------------------------
## Build the library

## [1] list of source files

set (dal_sources
  dalArray.cpp
  dalAttribute.cpp
  dalBaseTypes.cpp 
  dalColumn.cpp
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
endif (DAL_PYTHON_BINDINGS)

## [3] Build the library

add_library(dal STATIC ${dal_sources})

if (DAL_PYTHON_BINDINGS)
  add_library(pydal SHARED ${dal_sources})
#   SET_TARGET_PROPERTIES (pydal
#     PROPERTIES CMAKE_SHARED_LINKER_FLAGS_RELEASE "-o pydal"
#   )
endif (DAL_PYTHON_BINDINGS)

target_link_libraries(dal
  ${dal_link_libraries}
  )

#add_executable(msread msread.cpp)
#add_executable(mswrite mswrite.cpp)
#add_executable(time_series time_series.cpp)

#target_link_libraries(msread dal)
#target_link_libraries(mswrite dal)
#target_link_libraries(time_series dal)

#enable_testing()
#add_test(test1 ${CMAKE_CURRENT_BINARY_DIR}/mswrite foo.h5)
#add_test(test2 ${CMAKE_CURRENT_BINARY_DIR}/mswrite)

## ------------------------------------------------------------------------------
## Installation

## installation of libraries

install (
  TARGETS dal
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  )

install (
  TARGETS pydal
  LIBRARY DESTINATION lib
  )

## installation of header files

file (GLOB dal_headers *.h)

install (FILES ${dal_headers}
  DESTINATION include/dal
  )
