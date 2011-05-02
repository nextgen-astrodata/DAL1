
## ==============================================================================
##
##  Common setting for documentation generation
##
## ==============================================================================

## Load the the macro definition(s)
include (${PROJECT_SOURCE_DIR}/cmake/UseDoxygen.cmake)

## Generate list of source files

file (GLOB documentationSources *.doc *.tex *.in *.html)

list (APPEND documentationSources ${dalSources})
list (APPEND documentationSources ${dalHeaders})

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
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile${targetID}
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
  
  ## Define target to build User Manual
  add_custom_command (
    OUTPUT doc${targetID}/refman.pdf
    COMMAND make doc${targetID}
    COMMAND make doc${targetID}
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile${targetID}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
  add_custom_target(
    Name doc${targetID}
    COMMAND make doc${targetID}
    DEPENDS make_doc${targetID}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    SOURCES ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile${targetID}
    )
  
endif (PDFLATEX_EXECUTABLE AND LATEX_FNCYCHAP_PACKAGE AND LATEX_HYPERREF_PACKAGE)

