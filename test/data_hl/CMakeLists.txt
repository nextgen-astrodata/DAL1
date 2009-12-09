
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
##                                                            Testing preparation

find_program (BASH_EXCUTABLE bash ${bin_locations})

foreach (testScript
    tTBB_DipoleDataset
    tTBB_StationGroup
    tTBB_Timeseries)
  configure_file (${DAL_SOURCE_DIR}/test/data_hl/${testScript}.sh
    ${DAL_BINARY_DIR}/test/data_hl/${testScript}.sh
    )
endforeach (testScript)

##_______________________________________________________________________________
##                                                           Testing instructions

add_test (tBF_Dataset tBF_Dataset)
add_test (tBF_PencilBeam tBF_PencilBeam)
add_test (tBF_ProcessingHistory tBF_ProcessingHistory)
add_test (tBF_StationBeam tBF_StationBeam)
add_test (tBF_SysLog tBF_SysLog)

if (dataset_tbb)
  add_test (tTBB_DipoleDataset ${BASH_EXCUTABLE} tTBB_DipoleDataset.sh)
  add_test (tTBB_StationGroup ${BASH_EXCUTABLE} tTBB_StationGroup.sh)
  add_test (tTBB_Timeseries ${BASH_EXCUTABLE} tTBB_Timeseries.sh)
else (dataset_tbb)
  message (STATUS "Unable to find LOFAR time-series dataset; skipping tests.")
endif (dataset_tbb)
