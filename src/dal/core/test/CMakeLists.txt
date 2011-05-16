
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
    ${dal_link_libraries}
    )
  set_target_properties(${src}
                        PROPERTIES
			LINKER_LANGUAGE CXX
			)
  ## installation instructions
  install (TARGETS ${src}
    RUNTIME DESTINATION ${DAL_INSTALL_BINDIR}
    LIBRARY DESTINATION ${DAL_INSTALL_LIBDIR}
    )
endforeach (src)

##_______________________________________________________________________________
##                                                           Testing instructions

## Test with no further conditions required ______

foreach (_test
    tAccessMode
    tdalBaseTypes
    tdalCommon
    tdalCommon_operators
    tdalCommon_tbb
    tdalConversions
    tEnumerations
    tdalFileType
    tdalArray
    tdalDataset
    tdalFilter
    tdalGroup
    tDatabase
    tHDF5Dataset
    tValMatrix
    test_std_cerr
    )
  add_test (${_test} ${_test})
endforeach (_test)

## tdalCommon ____________________________________

if (dataset_tbb AND dataset_beamformed)
  add_test (tdalCommon_bf tdalCommon ${dataset_tbb} ${dataset_beamformed})
endif (dataset_tbb AND dataset_beamformed)

## tHDF5Object ___________________________________

if (HAVE_tHDF5Object)
  add_test (tHDF5Object    tHDF5Object    ${HAVE_tHDF5Object} )
  add_test (tHDF5Attribute tHDF5Attribute ${HAVE_tHDF5Object} )
  add_test (tHDF5Dataspace tHDF5Dataspace ${HAVE_tHDF5Object} )
  add_test (tHDF5Datatype  tHDF5Datatype  ${HAVE_tHDF5Object} )
else (HAVE_tHDF5Object)
  add_test (tHDF5Object    tHDF5Object    )
  add_test (tHDF5Attribute tHDF5Attribute )
  add_test (tHDF5Dataspace tHDF5Dataspace )
  add_test (tHDF5Datatype  tHDF5Datatype  )
endif (HAVE_tHDF5Object)

if (H5LS_EXECUTABLE)
  add_test (tHDF5Dataset_h5ls ${H5LS_EXECUTABLE} -f -r tHDF5Dataset.h5)
endif (H5LS_EXECUTABLE)

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

## test_CFITSIO __________________________________

if (CFITSIO_FOUND)
  add_test (test_CFITSIO test_CFITSIO)
endif (CFITSIO_FOUND)
