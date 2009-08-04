## ------------------------------------------------------------------------------
## $Id::                                                                        $
## ------------------------------------------------------------------------------

set(CMAKE_CXX_FLAGS "-W -Wall")
message(STATUS "cflags: ${CMAKE_CXX_FLAGS}")

## ==============================================================================
##
##  Build the test programs
##
## ==============================================================================

## Set up list of source files

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

## Build instructions

foreach (src ${tests})
  ## executable and its source
  add_executable(${src} ${src}.cpp)
  ## libraries against which to link
  if (HAVE_CASA OR HAVE_CASACORE)
    target_link_libraries(${src}
      ${CASA_LIBRARIES}
      dal
      ${CASA_LIBRARIES}
      ${CFITSIO_LIBRARIES}
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
  foreach (src tMeasurementSets tdalLopesEvent)
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

## [2] casacore & HDF5

if (HAVE_CASACORE AND HAVE_HDF5 AND GFORTRAN_LIBRARIES)

  foreach (src
      tMS_dal 
      tdalFITS
      tcasacore_hdf5
      tDirectionCoordinate
      tLinearCoordinate
      tTabularCoordinate
      tLOFAR_Image
      )
    ## compiler instructions
    add_executable (${src} ${src}.cpp)
    ## linker instructions
    target_link_libraries (${src}
      dal
      ${CASA_LIBRARIES}
      ${LAPACK_LIBRARIES}
      ${CFITSIO_LIBRARIES}
      ${WCSLIB_LIBRARIES}
      ${GFORTRAN_LIBRARIES}
      ${dal_link_libraries}
      )
    ## installation
    list (APPEND tests ${src})
    ## testing
    if (DAL_ENABLE_TESTING)
      add_test (${src} ${src})
    endif (DAL_ENABLE_TESTING)
  endforeach (src)
endif (HAVE_CASACORE AND HAVE_HDF5 AND GFORTRAN_LIBRARIES)

## ==============================================================================
##
##  Testing
##
## ==============================================================================

if (DAL_ENABLE_TESTING)
  
  ## -----------------------------------------------------------------
  ## Shell scripts which provide a wrapper around the test programs

  find_program (BASH_EXCUTABLE bash ${bin_locations})
  
  configure_file (${DAL_SOURCE_DIR}/test/tTBB_DipoleDataset.sh
    ${DAL_BINARY_DIR}/test/tTBB_DipoleDataset.sh
    )
  
  configure_file (${DAL_SOURCE_DIR}/test/tTBB_StationGroup.sh
    ${DAL_BINARY_DIR}/test/tTBB_StationGroup.sh
    )
  
  configure_file (${DAL_SOURCE_DIR}/test/tTBB_Timeseries.sh
    ${DAL_BINARY_DIR}/test/tTBB_Timeseries.sh
    )
  
  ## -----------------------------------------------------------------
  ## [1] simple tests which do not require sample data

  add_test(tdal tdal)
  add_test(tEnumerations tEnumerations)
  add_test(tLOFAR_Image tLOFAR_Image)
  add_test(tDirectionCoordinate tDirectionCoordinate)
  add_test(tLinearCoordinate tLinearCoordinate)
  add_test(tTabularCoordinate tTabularCoordinate)
  
  ## -----------------------------------------------------------------
  ## [2] Processing tests which require sample datasets
  
  if (dataset_ms)
    add_test (tMS_dal tMS_dal ${dataset_ms} ${dataset_ms}.h5)
  else (dataset_ms)
    message (STATUS "Unable to find MS dataset; skipping tests.")
  endif (dataset_ms)
  
  if (dataset_tbb AND dataset_beamformed)
    add_test (tdalCommon tdalCommon ${dataset_tbb} ${dataset_beamformed})
  endif (dataset_tbb AND dataset_beamformed)
  
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
    add_test (tTBB_test1 tTBB tTBB.h5)
    add_test (tTBB_test2 tTBB tTBB.h5 ${dataset_tbb})
    add_test (tTBB_DipoleDataset ${BASH_EXCUTABLE} tTBB_DipoleDataset.sh)
    add_test (tTBB_StationGroup ${BASH_EXCUTABLE} tTBB_StationGroup.sh)
    add_test (tTBB_Timeseries ${BASH_EXCUTABLE} tTBB_Timeseries.sh)
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
  else (dataset_beamformed)
    message (STATUS "Unable to find LOFAR beam-formed dataset; skipping tests.")
  endif (dataset_beamformed)

  ## -----------------------------------------------------------------
  ## [6] Tests using FITS file format data

  if (dataset_fits)
    add_test (tdal_fits tdal_fits ${dataset_fits})
  endif (dataset_fits)

  ## -----------------------------------------------------------------
  ## [7] Python scripts

  if (dataset_ms)
    ## basic test of Python wrappers
    add_test (dal_tests1 dal_tests.py)
    ## basic test of Python wrappers
    if (PYTHON_EXECUTABLE)
      add_test (dal_tests2 ${PYTHON_EXECUTABLE} dal_tests.py)
    endif (PYTHON_EXECUTABLE)
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

## scripts

file (GLOB python_scripts *.py)

install (FILES ${python_scripts}
  DESTINATION bin
  PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE OWNER_READ GROUP_READ WORLD_READ
)
