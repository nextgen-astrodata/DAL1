
cmake_minimum_required(VERSION 2.6)

##__________________________________________________________
## LOFAR components
find_package(LOFAR)

if (LOFAR_FOUND)
 include_directories (${LOFAR_INCLUDE_DIRECTORIES})
 add_definitions (-DHAVE_LOFAR)
endif (LOFAR_FOUND)

add_executable (bf2h5 bf2h5main.cpp StationBeamReader.cpp HDF5Writer.cpp Bf2h5Calculator.cpp bf2h5.cpp)

message(STATUS "BOOST_PROGRAM_OPTIONS_LIBRARY=${BOOST_PROGRAM_OPTIONS_LIBRARY}")

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
  