
##_______________________________________________________________________________
##                                                           List of source files

file (GLOB tests_data_hl *.cc)

##_______________________________________________________________________________
##                                                     Build/Install instructions

foreach (src ${tests_data_hl})
  ## get the name of the file without the extension
  get_filename_component (src ${src} NAME_WE)
  ## compiler instructions
  add_executable(${src} ${src}.cc)
  ## linker instructions
  target_link_libraries(${src}
    dal
    ${dal_link_libraries}
    )
  ## installation instructions
  install (TARGETS ${src}
    RUNTIME DESTINATION ${DAL_INSTALL_BINDIR}
    LIBRARY DESTINATION ${DAL_INSTALL_LIBDIR}
    )
endforeach (src)

##_______________________________________________________________________________
##                                                           Testing instructions

## run the actual tests

add_test (tAngle tAngle)
add_test (tRaDec tRaDec)
add_test (tStokes tStokes)
add_test (tCoordinate tCoordinate)
add_test (tCoordinateGenerator tCoordinateGenerator)
add_test (tCoordinatesGroup tCoordinatesGroup)

foreach (_test
    tDirectionCoordinate
    tLinearCoordinate
    tTabularCoordinate
    tStokesCoordinate
    )
  ## add test to run
  add_test (${_test} ${_test})
  ## check the generated HDF5 files
  if (H5CHECK_EXECUTABLE)
    add_test (h5check_${_test} ${H5CHECK_EXECUTABLE} --verbose=1 ${_test}.h5)
  endif (H5CHECK_EXECUTABLE)
  ## display statistics for the generated HDF5 file
  if (H5STAT_EXECUTABLE)
    add_test (h5stat_${_test} ${H5STAT_EXECUTABLE} --file ${_test}.h5)
  endif (H5STAT_EXECUTABLE)
endforeach (_test)

