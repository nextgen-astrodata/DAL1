
## ==============================================================================
##
##  Doxygen documentation for source code
##
## ==============================================================================

if (DOXYGEN_FOUND)
  
  ## Set up variables which will be filled in when parsing the input file for the
  ## Doxygen configuration
  
  set ( DOXYGEN_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_BINARY_DIR} )
  set ( DOXYGEN_CREATE_SUBDIRS    NO                          )
  set ( DOXYGEN_REPEAT_BRIEF      NO                          )
  set ( DOXYGEN_GENERATE_TREEVIEW NO                          )
  set ( DOXYGEN_GENERATE_MAN      NO                          )
  set ( DOXYGEN_HTML_FOOTER       ${CMAKE_CURRENT_SOURCE_DIR}/html_footer.html )
  
  ## Generate Doxygen configuration file
  
  configure_file (
    ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
    ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    )
  
  ## Define target to build documentation
  
  add_custom_target (Doc
    ${DOXYGEN_EXECUTABLE} Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating Doxygen code documentation ..."
    SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
    )
  
endif (DOXYGEN_FOUND)

## ==============================================================================
##
##  Manual  (LaTeX -> PDF)
##
## ==============================================================================

if (PDFLATEX_COMPILER)
  ## process LaTeX input into PDF directly
else (PDFLATEX_COMPILER)
  if (LATEX_COMPILER AND DVIPS_CONVERTER)
    ## process LaTeX input -> DVI -> PS
  endif (LATEX_COMPILER AND DVIPS_CONVERTER)
endif (PDFLATEX_COMPILER)
