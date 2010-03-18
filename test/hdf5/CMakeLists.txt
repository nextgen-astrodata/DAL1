
##_______________________________________________________________________________
##                                                           List of source files

file (GLOB tests_hdf5 tHDF5*.cc h5*.c ex_table*.c)
file (GLOB dal_hdf5_sources HDF5*.cc)

##_______________________________________________________________________________
##                                                     Build/Install instructions

add_library (dal_hdf5 ${dal_hdf5_sources})

foreach (src ${tests_hdf5})
  ## get the name of the file without the extension
  get_filename_component (extension ${src} EXT)
  get_filename_component (src ${src} NAME_WE)
  ## compiler instructions
  add_executable(${src} ${src}${extension})
  ## linker instructions
  target_link_libraries(${src}
    dal_hdf5
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

find_file (dataset L123456789_test_bf.h5
  PATHS ${USG_ROOT}
  PATH_SUFFIXES data/test
  )

## Test programs which come as part of the HDF5 library

add_test (h5_attribute h5_attribute)
add_test (h5_group h5_group)
add_test (h5_dataset_hyperslab h5_dataset_hyperslab)
add_test (h5_table_01 h5_table_01)
add_test (h5_table_02 h5_table_02)
add_test (h5_table_03 h5_table_03)
add_test (h5_table_04 h5_table_04)

## Test programs for the custom classes

if (dataset)
  add_test (tHDF5Attribute tHDF5Attribute ${dataset})
else (dataset_tbb)
  add_test (tHDF5Attribute tHDF5Attribute)
endif (dataset)
