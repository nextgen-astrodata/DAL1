
### === Build the test programs =================================================

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
    )
  
endforeach (src)

### === Run the test programs ===================================================

if (DAL_ENABLE_TESTING)

  add_test (tdal_MS tdal_MS)
  add_test (tcasacore_hdf5 tcasacore_hdf5)
  add_test (tcasacore_images tcasacore_images)
  add_test (tcasacore_images2 tcasacore_images2)
  
  if (TESTDATA_L20851_SB120)
    add_test (tcasacore_ms tcasacore_ms ${TESTDATA_L20851_SB120})
  else (TESTDATA_L20851_SB120)
    add_test (tcasacore_ms tcasacore_ms)
  endif (TESTDATA_L20851_SB120)
  
endif (DAL_ENABLE_TESTING)
