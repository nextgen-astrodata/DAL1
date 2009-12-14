
## ------------------------------------------------------------------------------
## $Id:: CMakeLists.txt 3828 2009-12-09 12:37:42Z baehren                       $
## ------------------------------------------------------------------------------

find_program (bash_executable bash ${bin_locations})

if (bash_executable)
  
  configure_file (dal_experimental_build.in dal_experimental_build)
  
  install (FILES ${DAL_BINARY_DIR}/scripts/dal_experimental_build
    DESTINATION bin
    PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE OWNER_READ GROUP_READ WORLD_READ
    )
  
endif (bash_executable)
