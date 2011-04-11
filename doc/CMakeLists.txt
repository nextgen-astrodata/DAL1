
## Find required packages for the generation of the documentation

set (DOXYGEN_FIND_QUIETLY YES )
set (LATEX_FIND_QUIETLY   YES )

include (FindDoxygen)
include (FindLATEX_DAL)

## Define common top-level target

add_custom_target (Documentation)

## ==============================================================================
##
##  Doxygen documentation for source code
##
## ==============================================================================

if (DOXYGEN_FOUND)
  
  ## Generate list of source files

  file (GLOB Documentation_sources *.doc)

  ## Set up variables which will be filled in when parsing the input file for the
  ## Doxygen configuration
  
  set ( DOXYGEN_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_BINARY_DIR} )
  set ( DOXYGEN_CREATE_SUBDIRS    "NO"                        )
  set ( DOXYGEN_REPEAT_BRIEF      "NO"                        )
  set ( DOXYGEN_GENERATE_TREEVIEW "NO"                        )
  set ( DOXYGEN_GENERATE_MAN      "NO"                        )
  set ( DOXYGEN_GENERATE_TODOLIST "YES"                                        )
  set ( DOXYGEN_GENERATE_TESTLIST "YES"                                        )
  set ( DOXYGEN_IMAGE_PATH        ${CMAKE_CURRENT_SOURCE_DIR}/figures          )
  set ( DOXYGEN_HTML_FOOTER       ${CMAKE_CURRENT_SOURCE_DIR}/html_footer.html )
  set ( DOXYGEN_LATEX_HEADER      ${CMAKE_CURRENT_SOURCE_DIR}/Manual-latex-header.tex )
  set ( DOXYGEN_WARN_LOGFILE      ${CMAKE_CURRENT_BINARY_DIR}/doxygen.log )
  
  ## Generate Doxygen configuration file
  
  configure_file (
    ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
    ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    )

  ## Define target to build documentation
  
  add_custom_command (
    OUTPUT html/index.html
    COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile ${Documentation_sources}
    COMMENT "Generating Doxygen code documentation ..."
    )
  
  add_custom_target (docReference ALL
    DEPENDS html/index.html
    )

  ## Installation of source code documentation

  install (
    DIRECTORY ${DOXYGEN_OUTPUT_DIRECTORY}/html
    DESTINATION ${DAL_INSTALL_DOCDIR}
    COMPONENT docReference
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

if (DOXYGEN_EXECUTABLE AND PDFLATEX_EXECUTABLE)

  if (LATEX_FNCYCHAP_PACKAGE)
  
  ## Create subdirectory 
  
  file (MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/manual)

  ## Create configuration file for Doxygen
  
  configure_file (
    ${CMAKE_CURRENT_SOURCE_DIR}/DoxyfileManual.in
    ${CMAKE_CURRENT_BINARY_DIR}/manual/Doxyfile
    )

  ## Generate Manual

  add_custom_command (
    OUTPUT refman.pdf
    COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile
    COMMAND ${PDFLATEX_EXECUTABLE} refman.tex
    COMMAND ${PDFLATEX_EXECUTABLE} refman.tex
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/manual
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile ${Documentation_sources}
    COMMENT "Generating User Manual ..."
    )
  
  add_custom_target (docManual ALL
    DEPENDS refman.pdf
    )

  ## Installation instructions

  install (
    FILES ${CMAKE_CURRENT_BINARY_DIR}/manual/refman.pdf
    DESTINATION ${DAL_INSTALL_DOCDIR}
    COMPONENT docManual
    RENAME UserManual-${DAL_VERSION}.pdf
    )

  ## Build target dependency

  add_dependencies (Documentation Manual)

  else (LATEX_FNCYCHAP_PACKAGE)

    message (STATUS "Skipping generation of user manual - missing LaTeX package!")

  endif (LATEX_FNCYCHAP_PACKAGE)
  
endif (DOXYGEN_EXECUTABLE AND PDFLATEX_EXECUTABLE)
