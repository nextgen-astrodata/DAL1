set(CMAKE_CXX_FLAGS "-W -Wall")
message(STATUS "cflags: ${CMAKE_CXX_FLAGS}")
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif(NOT CMAKE_BUILD_TYPE)


add_definitions(-DMPICH_IGNORE_CXX_SEEK -D_BSD_SOURCE -D_POSIX_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -D_LARGEFILE_SOURCE)

include_directories(
  /usr/local/include
  .
  ${DAL_SOURCE_DIR}/implement
  )

link_directories(/usr/local/lib)

set (tests
  msread
  mswrite
  time_series
  )

#add_executable(msread msread.cpp)
#add_executable(mswrite mswrite.cpp)
add_executable(time_series time_series.cpp)

#target_link_libraries(msread dal)
#target_link_libraries(mswrite dal)
target_link_libraries(time_series dal)

enable_testing()
add_test(test1 ${CMAKE_CURRENT_BINARY_DIR}/mswrite foo.h5)
add_test(test2 ${CMAKE_CURRENT_BINARY_DIR}/mswrite)
