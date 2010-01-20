
file (GLOB bf2h5_sources *.cpp)

## Compiler instructions
add_executable (bf2h5 ${bf2h5_sources})

## Linker instructions
target_link_libraries (bf2h5
  dal
  ${BOOST_PROGRAM_OPTIONS_LIBRARY}
  ${HDF5_LIBRARIES}
  )

## Installation instructions
install (TARGETS bf2h5
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  )

