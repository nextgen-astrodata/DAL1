## ------------------------------------------------------------------------------
## $Id::                                                                        $
## ------------------------------------------------------------------------------

## ==============================================================================
##
##  Common settings for all test programs
##
## ==============================================================================

set (CMAKE_CXX_FLAGS "-W -Wall")

macro (build_test arg_source arg_link arg_run)
  ## get the name of the file without the extension
  get_filename_component (_file ${arg_source} NAME_WE)
  ## compiler instructions
  add_executable(${_file} ${_file}.cc)
  ## linker instructions
  target_link_libraries (${_file} ${arg_link})
  ## installation instructions
  install (TARGETS ${_file}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    )
  ## run the test?
  if (arg_run)
    add_test (${_file} ${_file})
  endif (arg_run)
endmacro (build_test)

## ==============================================================================
##
##  Build the test programs
##
## ==============================================================================

add_subdirectory (hdf5)
add_subdirectory (core)
add_subdirectory (coordinates)
add_subdirectory (data_common)
add_subdirectory (data_hl)

## Set up list of source files

set (tests
  tdal
  tdal_fits
  tpulsar_dal
  tBeamFormed
  tBeamGroup
  tBeamSubband
  tEnumerations
  tTBB
  )

## Build instructions

foreach (src ${tests})
  ## executable and its source
  add_executable(${src} ${src}.cc)
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
    add_executable (${src} ${src}.cc)
    ## linker instructions
    target_link_libraries (${src} dal ${dal_link_libraries})
    ## installation
    list (APPEND tests ${src})
  endforeach (src)
else (HAVE_CASA OR HAVE_CASACORE)
  message (STATUS "Skipping test programs that require casacore!")
endif (HAVE_CASA OR HAVE_CASACORE)

## [2] casacore & HDF5

if (HAVE_CASACORE)
  if (HAVE_HDF5)
    if (GFORTRAN_LIBRARIES)
      if (LAPACK_LIBRARIES AND LAPACK_CBLAS_LIBRARY)
	## list of source files
	foreach (src
	    tMS_dal 
	    tcasacore_hdf5
	    tcasacore_images
	    tcasacore_images2
	    tLOFAR_Image
	    )
	  ## compiler instructions
	  add_executable (${src} ${src}.cc)
	  ## linker instructions
	  target_link_libraries (${src}
	    dal
	    ${dal_link_libraries}
	    ${CASA_LIBRARIES}
	    ${LAPACK_LIBRARIES}
	    ${CFITSIO_LIBRARIES}
	    ${WCSLIB_LIBRARIES}
	    ${GFORTRAN_LIBRARIES}
	    )
	  ## installation
	  list (APPEND tests ${src})
	  ## testing
	  if (DAL_ENABLE_TESTING)
	    add_test (${src} ${src})
	  endif (DAL_ENABLE_TESTING)
	endforeach (src)
      else (LAPACK_LIBRARIES AND LAPACK_CBLAS_LIBRARY)
	message (WARNING "Skipping tests - missing LAPACK.")
      endif (LAPACK_LIBRARIES AND LAPACK_CBLAS_LIBRARY)
    endif (LAPACK_LAPACK_LIBRARY)
  else (HAVE_HDF5)
    message (WARNING "Skipping tests - missing HDF5.")
  endif (HAVE_HDF5)
else (HAVE_CASACORE)
  message (WARNING "Skipping tests - missing casacore.")
endif (HAVE_CASACORE)

##__________________________________________________________
## tdalFITS

if (HAVE_CASACORE AND HAVE_HDF5 AND GFORTRAN_LIBRARIES AND LAPACK_LIBRARIES)

  ## compiler instructions
  add_executable (tdalFITS tdalFITS.cc)
  ## linker instructions
  target_link_libraries (tdalFITS
    dal
    ${dal_link_libraries}
    ${CASA_LIBRARIES}
    ${LAPACK_LIBRARIES}
    ${CFITSIO_LIBRARIES}
    ${WCSLIB_LIBRARIES}
    ${GFORTRAN_LIBRARIES}
    )
  ## installation
  list (APPEND tests tdalFITS)
  
  if (DAL_ENABLE_TESTING AND dataset_fits)
    add_test (tdalFITS tdalFITS ${dataset_fits})
  endif (DAL_ENABLE_TESTING AND dataset_fits)
  
endif (HAVE_CASACORE AND HAVE_HDF5 AND GFORTRAN_LIBRARIES AND LAPACK_LIBRARIES)

## ==============================================================================
##
##  Testing
##
## ==============================================================================

if (DAL_ENABLE_TESTING)
  
  ## -----------------------------------------------------------------
  ## [1] simple tests which do not require sample data

  add_test (tdal tdal)
  add_test (tEnumerations tEnumerations)

  ## -----------------------------------------------------------------
  ## [3] Processing tests which require sample datasets
  
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
    add_test (tTBB_test1 tTBB tTBB.h5)
    add_test (tTBB_test2 tTBB tTBB.h5 ${dataset_tbb})
  else (dataset_tbb)
    message (STATUS "Unable to find LOFAR time-series dataset; skipping tests.")
  endif (dataset_tbb)

  ## -----------------------------------------------------------------
  ## [5] Tests using HDF5-based beam-formed data

  if (dataset_beamformed)
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
  
  ## basic test of Python wrappers
  add_test (dal_tests1 dal_tests.py)
  ## basic test of Python wrappers
  if (PYTHON_EXECUTABLE)
    add_test (dal_tests2 ${PYTHON_EXECUTABLE} dal_tests.py)
  endif (PYTHON_EXECUTABLE)
  
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

## scripts

file (GLOB python_scripts *.py)

install (FILES ${python_scripts}
  DESTINATION bin
  PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE OWNER_READ GROUP_READ WORLD_READ
)
