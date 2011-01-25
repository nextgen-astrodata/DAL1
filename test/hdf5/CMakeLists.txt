
##_______________________________________________________________________________
##                                                           List of source files

file (GLOB tests_hdf5 tHDF5*.cc h5*.c h5*.cc ex_table*.c)

##_______________________________________________________________________________
##                                                     Build/Install instructions

foreach (src ${tests_hdf5})
  ## get the name of the file without the extension
  get_filename_component (extension ${src} EXT)
  get_filename_component (src ${src} NAME_WE)
  ## compiler instructions
  add_executable(${src} ${src}${extension})
  ## linker instructions
  target_link_libraries(${src}
    dal
    ${CASA_LIBRARIES}
    ${CFITSIO_LIBRARIES}
    ${dal_link_libraries}
    ${HAVE_MALLOC_DEBUG}
    )
  ## Instructions for running the test
  add_test (${src} ${src})
  ## Instructions for installation of the executable
  install (TARGETS ${src}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    )
endforeach (src)
