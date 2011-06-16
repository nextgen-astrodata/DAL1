
## ==============================================================================
##
##  Developer documentation
##
##  build
##  `-- doc
##      `-- developer
##          |-- html
##          `-- latex
##
## ==============================================================================

##__________________________________________________________  
## Generate Doxygen configuration file

set_doxygen_variables (${CMAKE_CURRENT_BINARY_DIR})

configure_file (
  ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
  ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
  )

##__________________________________________________________  
## Run Doxygen to generate documentation from sources

add_custom_target (DocumentationDeveloper
  COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile
  WORKING_DIRECTORY ${DOXYGEN_OUTPUT_DIRECTORY}
  )

set_property (DIRECTORY
  APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
  ${DOXYGEN_HTML_OUTPUT}
  )

if (LATEX_EXECUTABLE)
  add_custom_command(
    TARGET DocumentationDeveloper
    POST_BUILD
    COMMAND ${CMAKE_MAKE_PROGRAM}
    WORKING_DIRECTORY ${DOXYGEN_LATEX_OUTPUT}
    )
endif (LATEX_EXECUTABLE)

## Installation instructions
#  install(
#    DIRECTORY ${DOXYGEN_OUTPUT_DIRECTORY}
#    DESTINATION ${DAL_INSTALL_DOCDIR}
#    COMPONENT ${targetName}
#    )
