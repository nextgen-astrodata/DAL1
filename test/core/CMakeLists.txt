
##_______________________________________________________________________________
##                                                           List of source files

file (GLOB tests_core *.cc)

##_______________________________________________________________________________
##                                                     Build/Install instructions

foreach (src ${tests_core})
  ## get the name of the file without the extension
  get_filename_component (src ${src} NAME_WE)
  ## compiler instructions
  add_executable(${src} ${src}.cc)
  ## linker instructions
  target_link_libraries(${src}
    ${CASA_LIBRARIES}
    dal
    ${CASA_LIBRARIES}
    ${CFITSIO_LIBRARIES}
    ${dal_link_libraries}
    ${HAVE_MALLOC_DEBUG}
    )
  ## installation instructions
  install (TARGETS ${src}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    )
endforeach (src)

##_______________________________________________________________________________
##                                                           Testing instructions

## tdalDataset

if (dataset_ms)
  add_test (tdalDataset_ms tdalDataset ${dataset_ms})
endif (dataset_ms)

if (dataset_beamformed)
  add_test (tdalDataset_hdf5_bf tdalDataset ${dataset_beamformed})
endif (dataset_beamformed)

if (dataset_tbb)
  add_test (tdalDataset_hdf5_tbb tdalDataset ${dataset_tbb})
endif (dataset_tbb)

if (dataset_fits)
  add_test (tdalDataset_fits tdalDataset ${dataset_fits})
endif (dataset_fits)

## tdalGroup

add_test (tdalGroup tdalGroup)

## tdalTable

add_test (tdalTable1 tdalTable)

if (dataset_beamformed)
  add_test (tdalTable2 tdalTable ${dataset_beamformed})
endif (dataset_beamformed)

## tdalCommon
  
add_test (tdalCommon_test1 tdalCommon)

if (dataset_tbb AND dataset_beamformed)
  add_test (tdalCommon_test2 tdalCommon ${dataset_tbb} ${dataset_beamformed})
endif (dataset_tbb AND dataset_beamformed)
