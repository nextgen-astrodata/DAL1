set(CMAKE_CXX_FLAGS "-W -Wall")
message(STATUS "cflags: ${CMAKE_CXX_FLAGS}")

#if(NOT CMAKE_BUILD_TYPE)
#    set(CMAKE_BUILD_TYPE "Debug")
#endif(NOT CMAKE_BUILD_TYPE)

#add_definitions(
#  -DMPICH_IGNORE_CXX_SEEK
#  -D_BSD_SOURCE
#  -D_POSIX_SOURCE
#  -D_FILE_OFFSET_BITS=64
#  -D_LARGEFILE64_SOURCE
#  -D_LARGEFILE_SOURCE
#  )

## ------------------------------------------------------------------------------
## Build the test programs

set (tests
  tsread
  tswrite
  msread
  mswrite
  tdal
  tdal_fits
  tpulsar_dal
  )

foreach (src ${tests})
  ## executable and its source
  add_executable(${src} ${src}.cpp)
  ## libraries against which to link
  if (HAVE_CASA OR HAVE_CASACORE)
     target_link_libraries(${src} dal ${CASA_LIBRARIES})
  else (HAVE_CASA OR HAVE_CASACORE)
     target_link_libraries(${src} dal)
  endif (HAVE_CASA OR HAVE_CASACORE)
endforeach (src)

## ------------------------------------------------------------------------------
## Test which require additional external libraries (and thereby should be
## considered optional)

## [1] Blitz++

if (HAVE_BLITZ)
  foreach (src tdalLopesEvent lewrite)
    add_executable(${src} ${src}.cpp)
    target_link_libraries(${src} dal ${BLITZ_LIBRARIES})
    endforeach (src)
endif (HAVE_BLITZ)

## [2] casacore

if (HAVE_CASA OR HAVE_CASACORE)
  add_definitions (
    -DWITH_CASA
  )
  add_executable (tMeasurementSets tMeasurementSets.cpp)
  target_link_libraries (tMeasurementSets dal ${CASA_LIBRARIES} ${libm})
  add_executable (tMS_dal tMS_dal.cpp)
  target_link_libraries (tMS_dal dal ${CASA_LIBRARIES} ${libm})
  list (APPEND tests tMeasurementSets tMS_dal)
else (HAVE_CASA OR HAVE_CASACORE)
  message (STATUS "Skipping test program tMeasurementSets")
  message (STATUS "Skipping test program tMS_dal")
endif (HAVE_CASA OR HAVE_CASACORE)

## ------------------------------------------------------------------------------
## Testing

if (DAL_ENABLE_TESTING)
  add_test(test1 ${CMAKE_CURRENT_BINARY_DIR}/mswrite foo.h5)
  add_test(test2 ${CMAKE_CURRENT_BINARY_DIR}/mswrite)
endif (DAL_ENABLE_TESTING)

## ------------------------------------------------------------------------------
## Installation

## test programs

install (TARGETS ${tests}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
#   ARCHIVE DESTINATION lib
  )

## python scripts

install (FILES tdal.py tMS_dal.py uv_coverage.py baseline_intensity.py
  DESTINATION bin
  PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE OWNER_READ GROUP_READ WORLD_READ
)
