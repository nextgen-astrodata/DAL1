
include (FindDoxygen)

ADD_CUSTOM_TARGET (Documentation ALL)

## ==============================================================================
##
##  Doxygen documentation for source code
##
## ==============================================================================

if (DOXYGEN_FOUND)
  
  ## Set up variables which will be filled in when parsing the input file for the
  ## Doxygen configuration
  
  set ( DOXYGEN_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_BINARY_DIR} )
  set ( DOXYGEN_CREATE_SUBDIRS    "NO"                        )
  set ( DOXYGEN_REPEAT_BRIEF      "NO"                        )
  set ( DOXYGEN_GENERATE_TREEVIEW "NO"                        )
  set ( DOXYGEN_GENERATE_MAN      "NO"                        )
  set ( DOXYGEN_GENERATE_TODOLIST "YES"                       )
  set ( DOXYGEN_GENERATE_TESTLIST "YES"                       )
  set ( DOXYGEN_HTML_FOOTER       ${CMAKE_CURRENT_SOURCE_DIR}/html_footer.html )
  set ( DOXYGEN_LATEX_HEADER      ${CMAKE_CURRENT_SOURCE_DIR}/latex_header.tex )
  set ( DOXYGEN_WARN_LOGFILE      ${CMAKE_CURRENT_BINARY_DIR}/doxygen.log )
  
  ## Generate Doxygen configuration file
  
  configure_file (
    ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
    ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    )
  
  ## Define target to build documentation
  
  add_custom_target (DocumentationDoxygen
    ${DOXYGEN_EXECUTABLE} Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating Doxygen code documentation ..."
    SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
    ${CMAKE_CURRENT_SOURCE_DIR}/dal_installation.doc
    )

  add_dependencies (Documentation DocumentationDoxygen)

  ## Installation of source code documentation

  install (
    DIRECTORY ${DOXYGEN_OUTPUT_DIRECTORY}/html
    DESTINATION ${DAL_INSTALL_DOCDIR}
    )
  
else (DOXYGEN_FOUND)
  
  if (DOCUMENTATION_ONLY)
    message (FATAL_ERROR "Documentation only selected, but no Doxygen - install first and rerun CMake!")
  endif (DOCUMENTATION_ONLY)
  
endif (DOXYGEN_FOUND)

## ==============================================================================
##
##  Manual  (LaTeX -> PDF)
##
## ==============================================================================

if (DOXYGEN_FOUND)
  
  ## Create subdirectory 
  
  file (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/manual)

  ## Create configuration file for Doxygen
  
  configure_file (
    ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.Manual.in
    ${CMAKE_CURRENT_BINARY_DIR}/manual/Doxyfile
    )

  ## Generate Manual

  add_custom_target (Manual
    ${DOXYGEN_EXECUTABLE} Doxyfile
    COMMAND make
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/manual
    COMMENT "Generating Manual ..."
    SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.Manual.in
    ${CMAKE_CURRENT_SOURCE_DIR}/dal_overview.doc
    ${CMAKE_CURRENT_SOURCE_DIR}/dal_data_formats.doc
    )
  
endif (DOXYGEN_FOUND)
  
