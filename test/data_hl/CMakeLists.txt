
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
      dal
      ${dal_link_libraries}
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

if (dataset_tbb)
  
  find_program (BASH_EXCUTABLE bash ${bin_locations})
  get_filename_component (dataset_tbb_local ${dataset_tbb} NAME)
  
  foreach (testScript
      tTBB_DipoleDataset
      tTBB_StationGroup
      tTBB_Timeseries)
    configure_file (${DAL_SOURCE_DIR}/test/data_hl/${testScript}.sh
      ${DAL_BINARY_DIR}/test/data_hl/${testScript}.sh
      )
  endforeach (testScript)
  
endif (dataset_tbb)

##_______________________________________________________________________________
##                                                           Testing instructions

add_test (tBF_Dataset tBF_Dataset)
add_test (tBF_Beam tBF_Beam)
add_test (tBF_ProcessingHistory tBF_ProcessingHistory)
add_test (tBF_PrimaryPointing tBF_PrimaryPointing)
add_test (tSysLog tSysLog)
add_test (tTBB_StationTrigger tTBB_StationTrigger)

if (dataset_tbb)
  add_test (tTBB_DipoleDataset ${BASH_EXCUTABLE} tTBB_DipoleDataset.sh)
  add_test (tTBB_StationGroup ${BASH_EXCUTABLE} tTBB_StationGroup.sh)
  add_test (tTBB_Timeseries ${BASH_EXCUTABLE} tTBB_Timeseries.sh)
else (dataset_tbb)
  add_test (tTBB_DipoleDataset tTBB_DipoleDataset)
  add_test (tTBB_StationGroup tTBB_StationGroup)
  add_test (tTBB_Timeseries tTBB_Timeseries)
endif (dataset_tbb)

if (dataset_beamformed)
  add_test (tBeamFormed tBeamFormed ${dataset_beamformed})
  add_test (tBeamGroup tBeamGroup ${dataset_beamformed})
  add_test (tBeamSubband tBeamSubband ${dataset_beamformed})
endif (dataset_beamformed)

if (dataset_its)
  add_test (tITS_ExperimentMeta tITS_ExperimentMeta ${dataset_its})
else (dataset_its)
  add_test (tITS_ExperimentMeta tITS_ExperimentMeta)
endif (dataset_its)