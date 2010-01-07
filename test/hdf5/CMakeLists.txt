
##_______________________________________________________________________________
##                                                           List of source files

file (GLOB tests_hdf5 tH5*.cc)

##_______________________________________________________________________________
##                                                     Build/Install instructions

add_library (dal_hdf5
  H5Base.cc
  H5Dataset.cc
  H5Hyperslab.cc
  )

foreach (src ${tests_hdf5})
  ## get the name of the file without the extension
  get_filename_component (src ${src} NAME_WE)
  ## compiler instructions
  add_executable(${src} ${src}.cc)
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

add_test (tH5Dataset tH5Dataset)
add_test (tH5Hyperslab tH5Hyperslab)
