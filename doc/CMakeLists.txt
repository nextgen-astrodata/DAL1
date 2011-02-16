

## Set up variables which will be filled in when parsing the input file for the
## Doxygen configuration

set ( DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )
set ( DOXYGEN_CREATE_SUBDIRS   NO                          )
set ( DOXYGEN_REPEAT_BRIEF     NO                          )

## Generate Doxygen configuration file

configure_file (
  ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
  ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
  )

## Define target to build documentation

add_custom_target(Doc
  ${DOXYGEN_EXECUTABLE} Doxyfile
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  COMMENT "Generating Doxygen code documentation ..."
  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
  )
