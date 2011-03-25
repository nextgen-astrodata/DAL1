
##__________________________________________________________
## Build executable

set (bf2h5_sources
  bf2h5main.cpp
  StationBeamReader.cpp
  HDF5Writer.cpp 
  Bf2h5Calculator.cpp
  bf2h5.cpp
  )

if (Boost_PROGRAM_OPTIONS_LIBRARY)
  if (BOOST_DATE_TIME_LIBRARY)
    ## Compiler instructions
    add_executable (bf2h5 ${bf2h5_sources})
    ## Linker instructions
    target_link_libraries (bf2h5
      dal
      ${HDF5_LIBRARIES}
      ${LOFAR_LIBRARIES}
      ${Boost_PROGRAM_OPTIONS_LIBRARY}
      ${BOOST_DATE_TIME_LIBRARY}
      )
    ## Installation instructions
    install (TARGETS bf2h5
      RUNTIME DESTINATION ${DAL_INSTALL_BINDIR}
      LIBRARY DESTINATION ${DAL_INSTALL_LIBDIR}
      )
  else (BOOST_DATE_TIME_LIBRARY)
    message (STATUS "[DAL] Unable to build bf2h5 -- Boost data_time library!")
  endif (BOOST_DATE_TIME_LIBRARY)
else (Boost_PROGRAM_OPTIONS_LIBRARY)
  message (STATUS "[DAL] Unable to build bf2h5 -- Boost program_options library!")
endif (Boost_PROGRAM_OPTIONS_LIBRARY)
