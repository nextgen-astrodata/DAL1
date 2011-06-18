
## ==============================================================================
##
##  User Manual
##
##  build
##  `-- doc
##      `-- user
##          |-- html
##          `-- latex
##
## ==============================================================================

##__________________________________________________________  
## Find custom headers and footers

find_file (UserLatexHeader LatexHeader.tex)

if (NOT UserLatexHeader)
  set (UserLatexHeader "")
endif (NOT UserLatexHeader)

##__________________________________________________________  
## Generate Doxygen configuration file

set_doxygen_variables (${CMAKE_CURRENT_BINARY_DIR})

configure_file (
  ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
  ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
  )

##__________________________________________________________  
## Run Doxygen to generate documentation from sources

add_custom_target (DocumentationUser
  COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

set_property (DIRECTORY
  APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
  ${CMAKE_CURRENT_BINARY_DIR}/html
  )

if (LATEX_EXECUTABLE)
  add_custom_command(
    TARGET DocumentationUser
    POST_BUILD
    COMMAND ${CMAKE_MAKE_PROGRAM}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/latex
    )
endif (LATEX_EXECUTABLE)

## Installation instructions
#  install(
#    DIRECTORY ${DOXYGEN_OUTPUT_DIRECTORY}
#    DESTINATION ${DAL_INSTALL_DOCDIR}
#    COMPONENT ${targetName}
#    )
