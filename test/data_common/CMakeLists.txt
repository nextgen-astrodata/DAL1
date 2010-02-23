
##_______________________________________________________________________________
##                                                           List of source files

file (GLOB tests_data_common *.cc)

##_______________________________________________________________________________
##                                                     Build/Install instructions

foreach (src ${tests_data_common})
  ## get the name of the file without the extension
  get_filename_component (src ${src} NAME_WE)
  ## compiler instructions
  add_executable(${src} ${src}.cc)
  ## linker instructions
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
  ## installation instructions
  install (TARGETS ${src}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  )
endforeach (src)

##_______________________________________________________________________________
##                                                           Testing instructions

## C++ library

add_test (tTimestamp tTimestamp)
add_test (tFilename tFilename)
add_test (tCommonAttributes tCommonAttributes)
add_test (tSAS_Settings tSAS_Settings)
add_test (tHDF5Hyperslab tHDF5Hyperslab)
add_test (tHDF5Dataset tHDF5Dataset)
