
##__________________________________________________________
## Build executable

set (bf2h5_sources
  bf2h5main.cpp
  StationBeamReader.cpp
  HDF5Writer.cpp 
  Bf2h5Calculator.cpp
  bf2h5.cpp
  )

if (BOOST_PROGRAM_OPTIONS_LIBRARY)

  ## Compiler instructions
  add_executable (bf2h5 ${bf2h5_sources})
  
  ## Linker instructions
  if (HAVE_LOFAR)
    target_link_libraries (bf2h5
      dal
      ${BOOST_PROGRAM_OPTIONS_LIBRARY}
      ${HDF5_LIBRARIES}
      )
  else (HAVE_LOFAR)
    target_link_libraries (bf2h5
      dal
      ${BOOST_PROGRAM_OPTIONS_LIBRARY}
      ${HDF5_LIBRARIES}
      ${LOFAR_LIBRARIES}
      )
  endif (HAVE_LOFAR)  
  
  ## Installation instructions
  install (TARGETS bf2h5
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    )
  
else (BOOST_PROGRAM_OPTIONS_LIBRARY)
  message (STATUS "[DAL] Unable to build bf2h5 -- missing external dependencies.")
endif (BOOST_PROGRAM_OPTIONS_LIBRARY)
