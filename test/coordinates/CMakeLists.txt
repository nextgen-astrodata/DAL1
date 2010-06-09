
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
#    ${CASA_LIBRARIES}
#    ${LAPACK_LIBRARIES}
#    ${CFITSIO_LIBRARIES}
#    ${WCSLIB_LIBRARIES}
#    ${GFORTRAN_LIBRARIES}
    )
  ## installation instructions
  install (TARGETS ${src}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    )
endforeach (src)

##_______________________________________________________________________________
##                                                           Testing instructions

add_test (tAngle tAngle)
add_test (tRaDec tRaDec)
add_test (tCoordinateGenerator tCoordinateGenerator)
add_test (tDirectionCoordinate tDirectionCoordinate)
add_test (tLinearCoordinate tLinearCoordinate)
add_test (tStokesCoordinate tStokesCoordinate)
add_test (tTabularCoordinate tTabularCoordinate)
add_test (tCoordinatesGroup tCoordinatesGroup)
