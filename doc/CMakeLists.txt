
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

if (DOXYGEN_EXECUTABLE AND LATEX_EXECUTABLE)
  
  set (targetID                 "API")
  set (targetName               "doc${targetID}")
  set (DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/html")
  
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
    COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile${targetID}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
  
  ## Installation instructions
  install(
    DIRECTORY ${DOXYGEN_OUTPUT_DIRECTORY}
    DESTINATION ${DAL_INSTALL_DOCDIR}
    COMPONENT ${targetName}
    )
  
endif (DOXYGEN_EXECUTABLE AND LATEX_EXECUTABLE)

## ==============================================================================
##
##  Doxygen documentation for source code (HTML only)
##
## ==============================================================================

if (PDFLATEX_EXECUTABLE AND LATEX_FNCYCHAP_PACKAGE AND LATEX_HYPERREF_PACKAGE)

  set (targetID                 "Manual")
  set (targetName               "doc${targetID}")
  set (DOXYGEN_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${targetName}")
  set (targetOutput             "${DOXYGEN_OUTPUT_DIRECTORY}/refman.pdf")
  
  if (NOT EXISTS ${DOXYGEN_OUTPUT_DIRECTORY})
    file (MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIRECTORY})
  endif (NOT EXISTS ${DOXYGEN_OUTPUT_DIRECTORY})
  
  ## Generate Doxygen configuration time
  configure_file (
    ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile${targetID}.in
    ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile${targetID}
    )

  ## Define custom command to 
  add_custom_command (
    OUTPUT ${targetOutput}
    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile${targetID}
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile${targetID}
    WORKING_DIRECTORY ${DOXYGEN_OUTPUT_DIRECTORY}
    )

  ## Define target to build the documentation
  add_custom_target (${targetName}
    DEPENDS ${targetOutput}
    COMMAND ${PDFLATEX_EXECUTABLE} refman
    WORKING_DIRECTORY ${DOXYGEN_OUTPUT_DIRECTORY}
    )
  
  ## Installation instructions
#  install(
#    FILES ${targetOutput}
#    DESTINATION ${DAL_INSTALL_DOCDIR}
#    COMPONENT ${targetOutput}
#    RENAME UserManual-${DAL_VERSION}.pdf
#    )
  
endif (PDFLATEX_EXECUTABLE AND LATEX_FNCYCHAP_PACKAGE AND LATEX_HYPERREF_PACKAGE)

## ==============================================================================
##
##  Collective target to build documentation
##
## ==============================================================================

if (TARGET docManual)
  add_custom_target (Documentation
    COMMAND make docManual
    DEPENDS
    ${CMAKE_CURRENT_BINARY_DIR}/docManual/refman.pdf
    ${CMAKE_CURRENT_BINARY_DIR}/html/index.html
    )
else ()
  add_custom_target (Documentation
    DEPENDS
    ${DOXYGEN_OUTPUT_DIRECTORY}/index.html
    )
endif ()
