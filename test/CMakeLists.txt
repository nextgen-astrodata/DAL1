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

## [1] casacore

if (HAVE_CASA OR HAVE_CASACORE)
  ## additional compiler flags
  add_definitions (
    -DWITH_CASA
    )
  foreach (src tMeasurementSets tMS_dal tdalLopesEvent lewrite)
    ## compiler instructions
    add_executable (${src} ${src}.cpp)
    ## linker instructions
    target_link_libraries (${src} dal ${CASA_LIBRARIES} ${libm})
    ## installation
    list (APPEND tests ${src})
  endforeach (src)
else (HAVE_CASA OR HAVE_CASACORE)
  message (STATUS "Skipping test programs that require casacore!")
endif (HAVE_CASA OR HAVE_CASACORE)

## ------------------------------------------------------------------------------
## Testing

if (DAL_ENABLE_TESTING)

  ## [1] simple tests which do not require sample data

  add_test(test1 ${CMAKE_CURRENT_BINARY_DIR}/mswrite foo.h5)
  add_test(test2 ${CMAKE_CURRENT_BINARY_DIR}/mswrite)

  ## [2] Processing tests which require sample datasets
  
  ## Tests using MS datasets
  
  find_path (ms_dataset table.dat table.info
    PATHS
    ${DAL_SOURCE_DIR}/data
    ${DAL_SOURCE_DIR}/../data
    ${DAL_SOURCE_DIR}/../../data
    PATH_SUFFIXES
    L2007_01810_SB18-20_OB20.MS
    10602381_S0_T20.MS
    10602381_S0_T21.MS
    )
  
  if (ms_dataset)
    add_test (tMS_dal tMS_dal ${ms_dataset} ${ms_dataset}.h5)
  else (ms_dataset)
    message (STATUS "Unable to find MS dataset; skipping tests.")
  endif (ms_dataset)

  ## [3] Tests using LopesEvent datasets

  find_file (lopes_dataset 2007.01.31.23:59:33.960.event
    PATHS
    ${DAL_SOURCE_DIR}/data
    ${DAL_SOURCE_DIR}/../data
    ${DAL_SOURCE_DIR}/../../data
    PATH_SUFFIXES
    lopes
    LOPES
    )
  
  if (lopes_dataset)
    add_test (tdalLopesEvent tdalLopesEvent ${lopes_dataset})
  else (lopes_dataset)
    message (STATUS "Unable to find LopesEvent dataset; skipping tests.")
  endif (lopes_dataset)
  
endif (DAL_ENABLE_TESTING)

## ------------------------------------------------------------------------------
## Installation

## test programs

install (TARGETS ${tests}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  )

## python scripts

install (FILES tdal.py tMS_dal.py uv_coverage.py baseline_intensity.py datacube.py
  DESTINATION bin
  PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE OWNER_READ GROUP_READ WORLD_READ
)
