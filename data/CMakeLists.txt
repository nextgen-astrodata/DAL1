
file (GLOB dal_exampledata_hdf5 *.h5)

if (dal_exampledata_hdf5)
  
  foreach ( _dal_data ${dal_exampledata_hdf5} )

    ## Get filename components
    get_filename_component (_dataset_filename  ${_dal_data} NAME)
    get_filename_component (_dataset_name      ${_dal_data} NAME_WE)
    get_filename_component (_dataset_extension ${_dal_data} EXT)

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
    find_file (HAVE_${_dataset_name} ${_dataset_filename}
      PATHS ${CMAKE_CURRENT_BINARY_DIR}
      )
 
    ## Feedback
    if (HAVE_${_dataset_name})
      message (STATUS "Checking for dataset ${_dataset_filename} - Success")
    else (HAVE_${_dataset_name})
      message (STATUS "Checking for dataset ${_dataset_filename} - FAIL")
    endif (HAVE_${_dataset_name})
    
  endforeach (_dal_data)
  
endif (dal_exampledata_hdf5)
