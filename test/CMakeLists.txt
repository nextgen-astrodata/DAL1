## ------------------------------------------------------------------------------
## $Id::                                                                        $
## ------------------------------------------------------------------------------

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

## ==============================================================================
##
##  Build the test programs
##
## ==============================================================================

set (tests
  tdal
  tdal_fits
  tpulsar_dal
  tBeamFormed
  tBeamGroup
  tHDF5Common
  )

foreach (src ${tests})
  ## executable and its source
  add_executable(${src} ${src}.cpp)
  ## libraries against which to link
  if (HAVE_CASA OR HAVE_CASACORE)
    target_link_libraries(${src}
      dal
      ${dal_link_libraries}
      )
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
  foreach (src tMeasurementSets tMS_dal tdalLopesEvent)
    ## compiler instructions
    add_executable (${src} ${src}.cpp)
    ## linker instructions
    target_link_libraries (${src}  dal ${dal_link_libraries})
    ## installation
    list (APPEND tests ${src})
  endforeach (src)
else (HAVE_CASA OR HAVE_CASACORE)
  message (STATUS "Skipping test programs that require casacore!")
endif (HAVE_CASA OR HAVE_CASACORE)

## ==============================================================================
##
##  Testing
##
## ==============================================================================

if (DAL_ENABLE_TESTING)

  ## -----------------------------------------------------------------
  ## [1] simple tests which do not require sample data

  add_test(tdal ${CMAKE_CURRENT_BINARY_DIR}/tdal)

  ## -----------------------------------------------------------------
  ## [2] Processing tests which require sample datasets
  
  if (dataset_ms)
    add_test (tMS_dal tMS_dal ${dataset_ms} ${dataset_ms}.h5)
  else (dataset_ms)
    message (STATUS "Unable to find MS dataset; skipping tests.")
  endif (dataset_ms)

  ## -----------------------------------------------------------------
  ## [3] Tests using LopesEvent datasets

  find_file (lopes_dataset 2007.01.31.23:59:33.960.event
    PATHS
    ${DAL_SOURCE_DIR}/data
    ${DAL_SOURCE_DIR}/../data
    ${DAL_SOURCE_DIR}/../../data
    PATH_SUFFIXES
    lopes
    )
  
  if (lopes_dataset)
    add_test (tdalLopesEvent tdalLopesEvent ${lopes_dataset})
  else (lopes_dataset)
    message (STATUS "Unable to find LopesEvent dataset; skipping tests.")
  endif (lopes_dataset)

  ## -----------------------------------------------------------------
  ## [4] Tests using HDF5-based time-series dataset
  
  if (dataset_tbb)
    add_test (tHDF5Common tHDF5Common ${dataset_tbb})
  else (dataset_tbb)
    message (STATUS "Unable to find LOFAR time-series dataset; skipping tests.")
  endif (dataset_tbb)

  ## -----------------------------------------------------------------
  ## [5] Tests using HDF5-based beam-formed data

  if (dataset_beamformed)
    add_test (tBeamFormed tBeamFormed ${dataset_beamformed})
    add_test (tBeamGroup tBeamGroup ${dataset_beamformed})
  endif (dataset_beamformed)

  ## -----------------------------------------------------------------
  ## [6] Python scripts

  if (dataset_ms)
    ## show the coverage in the (u,v) plane
    add_test (uv_coverage uv_coverage.py ${dataset_ms})
    ## auto-correlated intensity in the first frequency band
    add_test (baseline_intensity1 baseline_intensity.py ${dataset_ms} 1 1 0)
    ## auto-correlated intensity in the first frequency band, channel 0
    add_test (baseline_intensity2 baseline_intensity.py ${dataset_ms} 1 1 0 0)
  endif (dataset_ms)
  
endif (DAL_ENABLE_TESTING)

## ------------------------------------------------------------------------------
## Installation

## test programs

install (TARGETS ${tests}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  )

## python scripts

install (FILES tdal.py tMS_dal.py uv_coverage.py baseline_intensity.py tBeamFormed.py
  DESTINATION bin
  PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE OWNER_READ GROUP_READ WORLD_READ
)
