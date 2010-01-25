
##__________________________________________________________
## Find LOFAR components

find_package(LOFAR)

if (LOFAR_FOUND)
 include_directories (${LOFAR_INCLUDE_DIRECTORIES})
 add_definitions (-DHAVE_LOFAR)
endif (LOFAR_FOUND)

##__________________________________________________________
## Build executable

if (LOFAR_FOUND AND BOOST_PROGRAM_OPTIONS_LIBRARY)

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
  
else (LOFAR_FOUND AND BOOST_PROGRAM_OPTIONS_LIBRARY)
  message (STATUS "[DAL] Unable to build bf2h5 -- missing external dependencies.")
endif (LOFAR_FOUND AND BOOST_PROGRAM_OPTIONS_LIBRARY)
