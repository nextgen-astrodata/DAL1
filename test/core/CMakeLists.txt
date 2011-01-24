
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

## Test with no further conditions required ______

foreach (_test
    tdalBaseTypes
    tdalCommon
    tdalCommon_operators
    tdalCommon_tbb
    tdalConversions
    tEnumerations
    tdalFileType
    tdalArray
    tdalFilter
    tdalGroup
    tDatabase
    tValMatrix
    )
  add_test (${_test} ${_test})
endforeach (_test)

## tdalCommon ____________________________________

if (dataset_tbb AND dataset_beamformed)
  add_test (tdalCommon_bf tdalCommon ${dataset_tbb} ${dataset_beamformed})
endif (dataset_tbb AND dataset_beamformed)

## tHDF5Object ___________________________________

find_file (HDF5OBJECT_H5 tHDF5Object.h5
  PATHS ${CMAKE_CURRENT_SOURCE_DIR}
  )

if (HDF5OBJECT_H5)
  add_test (tHDF5Object tHDF5Object ${HDF5OBJECT_H5})
else (HDF5OBJECT_H5)
  add_test (tHDF5Object tHDF5Object)
endif (HDF5OBJECT_H5)

## tdalDataset ___________________________________

if (dataset_ms)
  add_test (tdalDataset_ms tdalDataset ${dataset_ms} MSCASA)
endif (dataset_ms)

if (dataset_beamformed)
  add_test (tdalDataset_hdf5_bf tdalDataset ${dataset_beamformed})
endif (dataset_beamformed)

if (dataset_tbb)
  add_test (tdalDataset_hdf5_tbb tdalDataset ${dataset_tbb})
endif (dataset_tbb)

if (dataset_fits)
  add_test (tdalDataset_fits tdalDataset ${dataset_fits} FITS)
endif (dataset_fits)

## tdalTable _____________________________________

if (dataset_beamformed)
  add_test (tdalTable2 tdalTable ${dataset_beamformed})
else (dataset_beamformed)
  add_test (tdalTable2 tdalTable)
endif (dataset_beamformed)

