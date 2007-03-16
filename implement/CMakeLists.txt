set(CMAKE_CXX_FLAGS "-W -Wall")

message(STATUS "cflags: ${CMAKE_CXX_FLAGS}")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif(NOT CMAKE_BUILD_TYPE)

add_definitions (
  -DMPICH_IGNORE_CXX_SEEK
  -D_BSD_SOURCE
  -D_POSIX_SOURCE 
  -D_FILE_OFFSET_BITS=64
  -D_LARGEFILE64_SOURCE
  -D_LARGEFILE_SOURCE
  )

include_directories(
  .
  ${DAL_SOURCE_DIR}/implement
  ${HDF5_INCLUDES}
  ${CFITSIO_INCLUDES}
  )

link_directories(
  ${HDF5_LIBRARIES}
  ${CFITSIO_LIBRARIES}
  )


add_library(dal STATIC
  dalAttribute.cpp
  dalDataset.cpp
  dalTable.cpp
  dalGroup.cpp
  dalColumn.cpp
  dalRow.cpp
  dalBaseTypes.cpp 
#  l_member.cpp
  )

target_link_libraries(dal hdf5 hdf5_cpp hdf5_hl z m cfitsio)

#add_executable(msread msread.cpp)
#add_executable(mswrite mswrite.cpp)
#add_executable(time_series time_series.cpp)

#target_link_libraries(msread dal)
#target_link_libraries(mswrite dal)
#target_link_libraries(time_series dal)

#enable_testing()
#add_test(test1 ${CMAKE_CURRENT_BINARY_DIR}/mswrite foo.h5)
#add_test(test2 ${CMAKE_CURRENT_BINARY_DIR}/mswrite)
