
foreach (_dal_data
    tHDF5Object.h5
    )

  ## Get filename components
  get_filename_component (_dataset_name      ${_dal_data} NAME_WE)
  get_filename_component (_dataset_extension ${_dal_data} EXT)

  ## Check if the file is there
  if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_dal_data})

    ## copy file to build directory
    file (COPY ${_dal_data}
      DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
      )

    ## Set variable pointing to location of test data
    find_file (HAVE_${_dal_data} ${_dal_data}
      PATHS ${CMAKE_CURRENT_BINARY_DIR}
      )
    
  else (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_dal_data})
    
    message (STATUS "Unable to find dataset ${_dal_data} - skipping copy.")
    
  endif (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_dal_data})
  
endforeach (_dal_data)

