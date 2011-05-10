
##_______________________________________________________________________________
##                                                           List of source files

file (GLOB tests_hdf5 *.c *.cc)

##_______________________________________________________________________________
##                                                     Build/Install instructions

foreach (src ${tests_hdf5})
  ## get the name of the file without the extension
  get_filename_component (extension ${src} EXT)
  get_filename_component (src       ${src} NAME_WE)
  ## compiler instructions
  add_executable(${src} ${src}${extension})
  ## linker instructions
  if (MPI_LIBRARIES)
    target_link_libraries(${src}
      ${HDF5_LIBRARIES}
      ${MPI_LIBRARIES}
      )
  else (MPI_LIBRARIES)
    target_link_libraries(${src}
      ${HDF5_LIBRARIES}
      )
  endif (MPI_LIBRARIES)
  ## Instructions for running the test
  add_test (${src} ${src})
  ## Instructions for installation of the executable
  install (TARGETS ${src}
    RUNTIME DESTINATION ${DAL_INSTALL_BINDIR}
    LIBRARY DESTINATION ${DAL_INSTALL_LIBDIR}
    )
endforeach (src)
