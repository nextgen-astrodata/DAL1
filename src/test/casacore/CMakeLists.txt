
## list of source files
foreach (src
    tdal_MS 
    tcasacore_hdf5
    tcasacore_images
    tcasacore_images2
    tcasacore_ms
    )

  ## compiler instructions
  add_executable (${src} ${src}.cc)

  ## linker instructions
  target_link_libraries (${src}
    dal
    ${dal_link_libraries}
    ${GFORTRAN_LIBRARIES}
    )
  
  ## testing
  if (DAL_ENABLE_TESTING)
    add_test (${src} ${src})
  endif (DAL_ENABLE_TESTING)
  
endforeach (src)

## Additional tests taking input datasets

if (TESTDATA_L20851_SB120)
  add_test (tcasacore_ms2 tcasacore_ms ${TESTDATA_L20851_SB120})
endif (TESTDATA_L20851_SB120)
