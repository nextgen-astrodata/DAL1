set(CMAKE_CXX_FLAGS "-W -Wall")
message(STATUS "cflags: ${CMAKE_CXX_FLAGS}")
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif(NOT CMAKE_BUILD_TYPE)


add_definitions(
  -DMPICH_IGNORE_CXX_SEEK
  -D_BSD_SOURCE
  -D_POSIX_SOURCE
  -D_FILE_OFFSET_BITS=64
  -D_LARGEFILE64_SOURCE
  -D_LARGEFILE_SOURCE
  )

## location of the header files

include_directories(
  .
  ${DAL_SOURCE_DIR}/implement
  ${HDF5_INCLUDES}
  ${CFITSIO_INCLUDES}
  )

## ------------------------------------------------------------------------------
## Build the test programs

set (tests
  tsread
  tswrite
  msread
  mswrite
  tdalLopesEvent
  )

foreach (src ${tests})
  ## executable and its source
  add_executable(${src} ${src}.cpp)
  ## libraries against which to link
  target_link_libraries(${src}
    dal
    ${HDF5_LIBRARIES}
    )
endforeach (src)

## ------------------------------------------------------------------------------
## Testing

enable_testing()
add_test(test1 ${CMAKE_CURRENT_BINARY_DIR}/mswrite foo.h5)
add_test(test2 ${CMAKE_CURRENT_BINARY_DIR}/mswrite)
