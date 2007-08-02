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

link_directories(
  ${HDF5_LIBRARIES}
  ${BLITZ_LIBRARIES}
  )


add_library(dal STATIC
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

target_link_libraries(dal hdf5 hdf5_hl z m ${CFITSIO_LIBRARIES} ${BOOST_LIBRARIES} ${PYTHON_LIBRARIES})

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

## installation of header files

file (GLOB dal_headers *.h)

install (FILES ${dal_headers}
  DESTINATION include/dal
  )
