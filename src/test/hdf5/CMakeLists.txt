
##_______________________________________________________________________________
##                                                           List of source files

file (GLOB tests_hdf5 *.c *.cc)

## Test programs available through the HDF group website

set (hdf5_test_programs
  h5ex_g_compact.c
  h5ex_g_corder.c
  h5ex_g_create.c
  h5ex_g_intermediate.c
  h5ex_g_iterate.c
  h5ex_g_phase.c
  h5ex_g_traverse.c
  h5ex_g_visit.c
  )

##_______________________________________________________________________________
##                                                     Build/Install instructions

foreach (src ${tests_hdf5})
  ## get the name of the file without the extension
  get_filename_component (extension ${src} EXT)
  get_filename_component (src       ${src} NAME_WE)
  ## compiler instructions
  add_executable(${src} ${src}${extension})
  ## linker instructions
  target_link_libraries(${src}
    dal
    ${dal_link_libraries}
    )
  ## Instructions for running the test
  add_test (${src} ${src})
  ## Instructions for installation of the executable
  install (TARGETS ${src}
    RUNTIME DESTINATION ${DAL_INSTALL_BINDIR}
    LIBRARY DESTINATION ${DAL_INSTALL_LIBDIR}
    )
endforeach (src)
