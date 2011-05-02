
## ==============================================================================
##
##  Macro: add_doxygen_target
##
## ==============================================================================

macro (add_doxygen_target nameTarget nameDoxyfile)

  if (DOXYGEN_FOUND)

    ## Check for the Doxygen input file

    set(DOXYFILE_IN_${nameTarget})
    find_file(DOXYFILE_IN_${nameTarget} ${nameDoxyfile}.in
      PATHS ${PROJECT_SOURCE_DIR}
      PATH_SUFFIXES doc
      )

    ## Process Doxygen input file

    if (DOXYFILE_IN_${nameTarget})

      ## Set variables to be substituted 

      set ( DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )

      ## Generate Doxygen configuration file
      configure_file (
	${DOXYFILE_IN_${nameTarget}}
	${CMAKE_CURRENT_BINARY_DIR}/${nameDoxyfile}
	)

      ## Run Doxygen
      add_custom_command(
	OUTPUT make_${nameTarget}
        COMMAND ${DOXYGEN_EXECUTABLE} ARGS ${nameDoxyfile}
        DEPENDS ${nameDoxyfile}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating documentation target ${nameTarget} ..."
	)
      
      add_custom_target (${nameTarget}
	DEPENDS make_${nameTarget}
	)
      
      ## Testing instructions
      if (CMAKE_VERSION VERSION_GREATER 2.8.4)
	add_test (
	  NAME ${nameTarget}
	  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	  COMMAND make ${nameTarget}
	  )
      endif (CMAKE_VERSION VERSION_GREATER 2.8.4)
      
    else (DOXYFILE_IN_${nameTarget})
      add_custom_target (nameTarget
	WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
	COMMENT "Unable to build target ${nameTarget} - missing Doxygen input file!"
	)
    endif (DOXYFILE_IN_${nameTarget})    
    
  else (DOXYGEN_FOUND)
    add_custom_target (nameTarget
      WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
      COMMENT "Unable to build target ${nameTarget} - missing Doxygen!"
      )
  endif(DOXYGEN_FOUND)
  
endmacro ()
