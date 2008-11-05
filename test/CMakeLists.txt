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
  tdalCommon
  tdalDataset
  tdalTable
  tBeamFormed
  tBeamGroup
  tBeamSubband
  tEnumerations
  tTBB
  tTBB_DipoleDataset
  tTBB_StationGroup
  tTBB_Timeseries
  )

foreach (src ${tests})
  ## executable and its source
  add_executable(${src} ${src}.cpp)
  ## libraries against which to link
  if (HAVE_CASA OR HAVE_CASACORE)
    target_link_libraries(${src}
      dal
      ${dal_link_libraries}
      ${HAVE_MALLOC_DEBUG}
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
  add_definitions (-DHAVE_CASA)
  foreach (src tMeasurementSets tMS_dal tdalLopesEvent)
    ## compiler instructions
    add_executable (${src} ${src}.cpp)
    ## linker instructions
    target_link_libraries (${src} dal ${dal_link_libraries})
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

  add_test(tdal tdal)
  add_test(tEnumerations tEnumerations)

  ## -----------------------------------------------------------------
  ## [2] Processing tests which require sample datasets
  
  if (dataset_ms)
    add_test (tMS_dal tMS_dal ${dataset_ms} ${dataset_ms}.h5)
  else (dataset_ms)
    message (STATUS "Unable to find MS dataset; skipping tests.")
  endif (dataset_ms)

  ## -----------------------------------------------------------------
  ## [3] Tests using LopesEvent datasets

  if (dataset_lopes)
    add_test (tdalLopesEvent tdalLopesEvent ${dataset_lopes})
  else (dataset_lopes)
    message (STATUS "Unable to find LopesEvent dataset; skipping tests.")
  endif (dataset_lopes)

  ## -----------------------------------------------------------------
  ## [4] Tests using HDF5-based time-series dataset
  
  if (dataset_tbb)
    add_test (tdalCommon tdalCommon ${dataset_tbb})
    add_test (tTBB_DipoleDataset tTBB_DipoleDataset ${dataset_tbb})
    add_test (tTBB_StationGroup tTBB_StationGroup ${dataset_tbb})
    add_test (tTBB_Timeseries tTBB_Timeseries ${dataset_tbb})
  else (dataset_tbb)
    message (STATUS "Unable to find LOFAR time-series dataset; skipping tests.")
  endif (dataset_tbb)

  ## -----------------------------------------------------------------
  ## [5] Tests using HDF5-based beam-formed data

  if (dataset_beamformed)
    add_test (tdalDataset_test1 tdalDataset ${dataset_beamformed})
    add_test (tdalDataset_test2 tdalDataset ${dataset_tbb})
    add_test (tdalTable tdalTable ${dataset_beamformed})
    add_test (tBeamFormed tBeamFormed ${dataset_beamformed})
    add_test (tBeamGroup tBeamGroup ${dataset_beamformed})
    add_test (tBeamSubband tBeamSubband ${dataset_beamformed})
  endif (dataset_beamformed)

  ## -----------------------------------------------------------------
  ## [6] Tests using FITS file format data

  if (dataset_fits)
    add_test (tdal_fits tdal_fits ${dataset_fits})
  endif (dataset_fits)

  ## -----------------------------------------------------------------
  ## [7] Python scripts

  if (dataset_ms)
    ## show the coverage in the (u,v) plane
    add_test (datacube datacube.py ${dataset_ms} ${dataset_ms}.out 1 1 0)
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

file (GLOB python_scripts *.py)

install (FILES ${python_scripts}
  DESTINATION bin
  PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE OWNER_READ GROUP_READ WORLD_READ
)
