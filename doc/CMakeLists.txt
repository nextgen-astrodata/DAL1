
## ==============================================================================
##
##  Common setting for documentation generation
##
## ==============================================================================

##____________________________________________________________________
## Generate list of source files

file (GLOB documentationSources *.doc *.tex *.in *.html)

##____________________________________________________________________
## Macro definition(s)

macro (set_doxygen_variables _basedir)
  
  ## Root directory for Doxygen output
  set ( DOXYGEN_OUTPUT_DIRECTORY  ${_basedir}            )
  ## Directory for HTML output
  set ( DOXYGEN_HTML_OUTPUT       ${_basedir}/html       )
  ## Directory for LaTeX output
  set ( DOXYGEN_LATEX_OUTPUT      ${_basedir}/latex      )
  ## Doxygen logfile with warning messages
  set ( DOXYGEN_WARN_LOGFILE     ${_basedir}/doxygen.log )
  
endmacro (set_doxygen_variables)

##____________________________________________________________________
## Set up variables which will be filled in when parsing the input
## file for the Doxygen configuration.

set ( DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}                         )
set ( DOXYGEN_IMAGE_PATH       ${CMAKE_CURRENT_SOURCE_DIR}/figures                 )
set ( DOXYGEN_HTML_FOOTER      ${CMAKE_CURRENT_SOURCE_DIR}/html_footer.html        )
set ( DOXYGEN_LATEX_HEADER     ${CMAKE_CURRENT_SOURCE_DIR}/Manual-latex-header.tex )

##____________________________________________________________________
## Enable/Disable support for LaTeX

if (LATEX_EXECUTABLE)
  
  set (DOXYGEN_GENERATE_LATEX "YES" )
  set (DOXYGEN_EXTRA_PACKAGES ""    )
  
  if (LATEX_A4WIDE_PACKAGE)
    set (DOXYGEN_EXTRA_PACKAGES "a4wide")
  endif ()
  
  if (LATEX_FANCYHDR_PACKAGE)
    set (DOXYGEN_EXTRA_PACKAGES "${DOXYGEN_EXTRA_PACKAGES} fancyhdr")
  endif ()
  
else (LATEX_EXECUTABLE)

  set ( DOXYGEN_GENERATE_LATEX "NO" )

endif (LATEX_EXECUTABLE)

## === Developer documentation ==================================================

add_subdirectory (developer)

## ==============================================================================
##
##  Doxygen documentation for source code (HTML only)
##
## ==============================================================================

if (PDFLATEX_EXECUTABLE AND LATEX_FNCYCHAP_PACKAGE AND LATEX_HYPERREF_PACKAGE)

  set_doxygen_variables (${CMAKE_CURRENT_BINARY_DIR})

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
    COMMAND ${PDFLATEX_EXECUTABLE} refman
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
