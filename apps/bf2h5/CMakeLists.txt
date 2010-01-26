
##__________________________________________________________
## Find LOFAR components

find_package(LOFAR)

if (HAVE_LOFAR)
 include_directories (${LOFAR_INCLUDES})
 add_definitions (-DHAVE_LOFAR)
endif (HAVE_LOFAR)

##__________________________________________________________
## Build executable

if (HAVE_LOFAR AND BOOST_PROGRAM_OPTIONS_LIBRARY)

  ## Compiler instructions
  add_executable (bf2h5
    bf2h5main.cpp
    StationBeamReader.cpp
    HDF5Writer.cpp 
    Bf2h5Calculator.cpp
    bf2h5.cpp
    )

  ## Linker instructions
  target_link_libraries (bf2h5
    dal
    ${BOOST_PROGRAM_OPTIONS_LIBRARY}
    ${HDF5_LIBRARIES}
    ${LOFAR_LIBRARIES}
    )
  
  ## Installation instructions
  install (TARGETS bf2h5
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    )
  
else (HAVE_LOFAR AND BOOST_PROGRAM_OPTIONS_LIBRARY)
  message (STATUS "[DAL] Unable to build bf2h5 -- missing external dependencies.")
endif (HAVE_LOFAR AND BOOST_PROGRAM_OPTIONS_LIBRARY)
