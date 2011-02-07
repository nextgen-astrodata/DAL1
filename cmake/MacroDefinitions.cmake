
## ==============================================================================
##
##  Definitions of macros
##
## ==============================================================================

macro (build_test arg_source arg_link arg_run)
  ## get the name of the file without the extension
  get_filename_component (_file ${arg_source} NAME_WE)
  ## compiler instructions
  add_executable(${_file} ${_file}.cc)
  ## linker instructions
  target_link_libraries (${_file} ${arg_link})
  ## installation instructions
  install (TARGETS ${_file}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    )
  ## run the test?
  if (arg_run)
    add_test (${_file} ${_file})
  endif (arg_run)
endmacro (build_test)
