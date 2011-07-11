
file (GLOB dalExampledata *.h5 *.ms)

if (dalExampledata)
  
  foreach ( _dal_data ${dalExampledata} )

    ## Get filename components
    get_filename_component (_dataset_filename  ${_dal_data} NAME)
    get_filename_component (_dataset_name      ${_dal_data} NAME_WE)
    get_filename_component (_dataset_extension ${_dal_data} EXT)

    ## Generate name of variable passed on
    string (TOUPPER ${_dataset_name} _datasetVar)
    set (_datasetVar TESTDATA_${_datasetVar})

    message (STATUS "Checking for dataset ${_dataset_filename}")
    
    ## copy file to build directory
    if (${CMAKE_VERSION} VERSION_LESS 2.8.4)
      execute_process(
	COMMAND cp ${_dal_data} .
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	)
    else (${CMAKE_VERSION} VERSION_LESS 2.8.4)
      file (COPY ${_dal_data}
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
	)
    endif(${CMAKE_VERSION} VERSION_LESS 2.8.4)

    ## Set variable pointing to location of test data
    find_file (${_datasetVar} ${_dataset_filename}
      PATHS ${CMAKE_CURRENT_BINARY_DIR}
      )

    ## Feedback
    if (_datasetVar)
      message (STATUS "Checking for dataset ${_dataset_filename} - Success")
    else (_datasetVar)
      message (STATUS "Checking for dataset ${_dataset_filename} - FAIL")
    endif (_datasetVar)
    
  endforeach (_dal_data)
  
endif (dalExampledata)
