
## ==============================================================================
##
##  Common setting for documentation generation
##
## ==============================================================================

## Load the the macro definition(s)
include (${PROJECT_SOURCE_DIR}/cmake/UseDoxygen.cmake)

## Generate list of source files

file (GLOB documentationSources *.doc *.tex *.in *.html)

## Set up variables which will be filled in when parsing the input file for the
## Doxygen configuration

set ( DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}                         )
set ( DOXYGEN_IMAGE_PATH       ${CMAKE_CURRENT_SOURCE_DIR}/figures                 )
set ( DOXYGEN_HTML_FOOTER      ${CMAKE_CURRENT_SOURCE_DIR}/html_footer.html        )
set ( DOXYGEN_LATEX_HEADER     ${CMAKE_CURRENT_SOURCE_DIR}/Manual-latex-header.tex )
set ( DOXYGEN_WARN_LOGFILE     ${CMAKE_CURRENT_BINARY_DIR}/doxygen.log             )

## ==============================================================================
##
##  Doxygen documentation for source code (HTML only)
##
## ==============================================================================

## Process the inputs to the projects

if (LATEX_EXECUTABLE)
  
  set (targetID                 "API")
  set (targetName               "doc${targetID}")
  set (DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${targetName}")
  
  ## Generate Doxygen configuration time
  configure_file (
    ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile${targetID}.in
    ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile${targetID}
    )

  ## Define custom command to 
  add_custom_command (
    OUTPUT ${DOXYGEN_OUTPUT_DIRECTORY}/index.html
    COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile${targetID}
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile${targetID} ${dalHeaders}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )

  ## Define target to build the documentation
  add_custom_target (${targetName}
    DEPENDS ${DOXYGEN_OUTPUT_DIRECTORY}/index.html
    )
  
endif (LATEX_EXECUTABLE)

## ==============================================================================
##
##  Doxygen documentation for source code (HTML only)
##
## ==============================================================================

if (PDFLATEX_EXECUTABLE AND LATEX_FNCYCHAP_PACKAGE AND LATEX_HYPERREF_PACKAGE)

  set (targetID                 "Manual")
  set (targetName               "doc${targetID}")
  set (DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${targetName}")
  
  ## Generate Doxygen configuration time
  configure_file (
    ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile${targetID}.in
    ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile${targetID}
    )

  ## Define custom command to 
  add_custom_command (
    OUTPUT ${DOXYGEN_OUTPUT_DIRECTORY}/refman.pdf
    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile${targetID}
    COMMAND ${PDFLATEX_EXECUTABLE} refman
    COMMAND ${PDFLATEX_EXECUTABLE} refman
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile${targetID}
    WORKING_DIRECTORY ${DOXYGEN_OUTPUT_DIRECTORY}
    )

  ## Define target to build the documentation
  add_custom_target (${targetName}
    COMMAND make ${targetName}
    DEPENDS ${DOXYGEN_OUTPUT_DIRECTORY}/refman.pdf
    )
  
  ## Installation instructions
  install(
    FILES ${DOXYGEN_OUTPUT_DIRECTORY}/refman.pdf
    DESTINATION ${DAL_INSTALL_DOCDIR}
    COMPONENT ${targetName}
    RENAME UserManual-${DAL_VERSION}.pdf
    )
  
endif (PDFLATEX_EXECUTABLE AND LATEX_FNCYCHAP_PACKAGE AND LATEX_HYPERREF_PACKAGE)

## ==============================================================================
##
##  Collective target to build documentation
##
## ==============================================================================

if (TARGET docManual)
  add_custom_target (Documentation
    DEPENDS ${DOXYGEN_OUTPUT_DIRECTORY}/refman.pdf
    )
endif ()