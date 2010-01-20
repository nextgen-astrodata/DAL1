
file (GLOB bf2h5_sources *.cpp)

add_executable (bf2h5 ${bf2h5_sources})

target_link_libraries (bf2h5
  dal
  ${BOOST_PROGRAM_OPTIONS_LIBRARY}
  ${HDF5_LIBRARIES}
  )
