
##_______________________________________________________________________________
##                                                           List of source files

set (tests_data_common 
  tCommonAttributes.cc
  tFilename.cc
  tHDF5Measure.cc
  tHDF5Quantity.cc
  tSAS_Settings.cc
  tTimestamp.cc
  )

##_______________________________________________________________________________
##                                                     Build/Install instructions

foreach (src ${tests_data_common})
  ## get the name of the file without the extension
  get_filename_component (src ${src} NAME_WE)
  ## compiler instructions
  add_executable(${src} ${src}.cc)
  ## linker instructions
  if (CASA_FOUND OR CASACORE_FOUND)
    target_link_libraries(${src}
      dal
      ${dal_link_libraries}
      )
  else (CASA_FOUND OR CASACORE_FOUND)
    target_link_libraries(${src} dal)
  endif (CASA_FOUND OR CASACORE_FOUND)
endforeach (src)

##_______________________________________________________________________________
##                                                           Testing instructions

## C++ library

add_test (tHDF5Quantity tHDF5Quantity )
add_test (tHDF5Measure  tHDF5Measure  )
add_test (tTimestamp    tTimestamp    )
add_test (tFilename     tFilename     )
add_test (tSAS_Settings tSAS_Settings )
add_test (tCommonAttributes tCommonAttributes)

if (H5DUMP_EXECUTABLE)
  add_test (tCommonAttributes_h5dump ${H5DUMP_EXECUTABLE} tCommonAttributes.h5)
endif (H5DUMP_EXECUTABLE)

if (H5LS_EXECUTABLE)
  add_test (tCommonAttributes_h5ls ${H5LS_EXECUTABLE} tCommonAttributes.h5)
endif (H5LS_EXECUTABLE)
