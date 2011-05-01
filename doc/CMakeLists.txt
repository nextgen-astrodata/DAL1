
## ==============================================================================
##
##  Macro definitions
##
## ==============================================================================

macro (add_doxygen_target nameTarget nameDoxyfile)
  
  if (DOXYGEN_FOUND)

    ## Check for the Doxygen input file

    set(DOXYFILE_IN_${nameTarget})
    find_file(DOXYFILE_IN_${nameTarget} ${nameDoxyfile}.in
      PATHS ${PROJECT_SOURCE_DIR}
      PATH_SUFFIXES doc
      )

    ## Process Doxygen input file

    if (DOXYFILE_IN_${name})
      
      add_custom_command(TARGET ${nameTarget}
        POST_BUILD
        COMMAND ${CMAKE_MAKE_PROGRAM}
        WORKING_DIRECTORY "${DOXYFILE_OUTPUT_DIR}/${DOXYFILE_LATEX_DIR}")
      
    else (DOXYFILE_IN_${name})
      add_custom_target (nameTarget
	WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
	COMMENT "Unable to build target ${nameTarget} - missing Doxygen input file!"
	)
    endif (DOXYFILE_IN_${name})    
    
  else (DOXYGEN_FOUND)
    add_custom_target (nameTarget
      WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
      COMMENT "Unable to build target ${nameTarget} - missing Doxygen!"
      )
  endif(DOXYGEN_FOUND)
  
endmacro ()

## ==============================================================================
##
##  Doxygen documentation for source code (HTML only)
##
## ==============================================================================

add_doxygen_target (docAPI DoxyfileAPI)

## Generate list of source files

file (GLOB Documentation_sources *.doc *.tex *.in *.html)

## Set up variables which will be filled in when parsing the input file for the
## Doxygen configuration

set ( DOXYGEN_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_BINARY_DIR} )
set ( DOXYGEN_IMAGE_PATH        ${CMAKE_CURRENT_SOURCE_DIR}/figures          )
set ( DOXYGEN_HTML_FOOTER       ${CMAKE_CURRENT_SOURCE_DIR}/html_footer.html )
set ( DOXYGEN_LATEX_HEADER      ${CMAKE_CURRENT_SOURCE_DIR}/Manual-latex-header.tex )
set ( DOXYGEN_WARN_LOGFILE      ${CMAKE_CURRENT_BINARY_DIR}/doxygen.log )

## Generate Doxygen configuration file

configure_file (
  ${CMAKE_CURRENT_SOURCE_DIR}/DoxyfileAPI.in
  ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
  )

## Define target to build documentation

add_custom_command (
  OUTPUT html/index.html
  COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile ${Documentation_sources} ${dalSources}
  COMMENT "Generating Doxygen code documentation ..."
  )

add_custom_target (docReference
  DEPENDS html/index.html
  )

## Testing instructions
if (CMAKE_VERSION VERSION_GREATER 2.8.4)
  add_test (
    NAME docReference
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND make docReference
    )
endif (CMAKE_VERSION VERSION_GREATER 2.8.4)

## Installation of source code documentation

#if (TARGET docReference)
#  install (
#    DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html
#    DESTINATION ${DAL_INSTALL_DOCDIR}
#    COMPONENT docReference
#    )
#endif (TARGET docReference)

## ==============================================================================
##
##  Manual  (LaTeX -> PDF)
##
## ==============================================================================

if (PDFLATEX_EXECUTABLE)
  if (LATEX_FNCYCHAP_PACKAGE)
    if (LATEX_HYPERREF_PACKAGE)
      
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
	COMMAND ${PDFLATEX_EXECUTABLE} refman.tex
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/manual
	DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile ${Documentation_sources}
	COMMENT "Generating User Manual ..."
	)
      
      add_custom_target (
	TARGET docManual
	POST_BUILD
	DEPENDS refman.pdf
	)
      
      ## Testing instructions
      if (CMAKE_VERSION VERSION_GREATER 2.8.4)
	add_test (
	  NAME docManual
	  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/manual
	  COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile; ${PDFLATEX_EXECUTABLE} refman.tex
	  )
      endif (CMAKE_VERSION VERSION_GREATER 2.8.4)
      
      ## Installation instructions
      
      install (
	FILES refman.pdf
	DESTINATION ${DAL_INSTALL_DOCDIR}
	RENAME UserManual-${DAL_VERSION}.pdf
	)
      
    else (LATEX_HYPERREF_PACKAGE)
      message (STATUS "Skipping generating user manual - missing LaTeX package hyperref!")
    endif (LATEX_HYPERREF_PACKAGE)
    
  else (LATEX_FNCYCHAP_PACKAGE)
    message (STATUS "Skipping generating user manual - missing LaTeX package fncychap!")
  endif (LATEX_FNCYCHAP_PACKAGE)
  
else (PDFLATEX_EXECUTABLE)
  message (STATUS "Skipping generating user manual - missing pdfLaTeX!")
endif (PDFLATEX_EXECUTABLE)

## ==============================================================================
##
##  Collective target to build all available parts of documentation
##
## ==============================================================================

if (TARGET docManual)
  add_custom_target (Documentation
    DEPENDS html/index.html refman.pdf
    )
else (TARGET docManual)
  add_custom_target (Documentation
    DEPENDS html/index.html
    )
endif (TARGET docManual)
