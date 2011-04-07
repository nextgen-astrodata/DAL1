# +-----------------------------------------------------------------------------+
# |   Copyright (C) 2007                                                        |
# |   Lars B"ahren (bahren@astron.nl)                                           |
# |                                                                             |
# |   This program is free software; you can redistribute it and/or modify      |
# |   it under the terms of the GNU General Public License as published by      |
# |   the Free Software Foundation; either version 2 of the License, or         |
# |   (at your option) any later version.                                       |
# |                                                                             |
# |   This program is distributed in the hope that it will be useful,           |
# |   but WITHOUT ANY WARRANTY; without even the implied warranty of            |
# |   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             |
# |   GNU General Public License for more details.                              |
# |                                                                             |
# |   You should have received a copy of the GNU General Public License         |
# |   along with this program; if not, write to the                             |
# |   Free Software Foundation, Inc.,                                           |
# |   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                 |
# +-----------------------------------------------------------------------------+

# - Check for the presence of LATEX
#
# The following variables are set when LATEX is found:
#  LATEX_FOUND      = Set to true, if all components of LATEX
#                         have been found.
#  LATEX_INCLUDES   = Include path for the header files of LATEX
#  LATEX_LIBRARIES  = Link these to use LATEX
#  LATEX_LFLAGS     = Linker flags (optional)

if (NOT LATEX_FOUND)
    
  if (NOT LATEX_ROOT_DIR)
    set (LATEX_ROOT_DIR ${CMAKE_INSTALL_PREFIX})
  endif (NOT LATEX_ROOT_DIR)
  
  ##_____________________________________________________________________________
  ## Check for the executable

  foreach (_latexExecutable
      latex
      dvipdfmx
      dvipdf
      dvips
      pdflatex
      pstopdf
      ps2pdf
      biblatex
      bibtex
      eps2eps
      kpsepath
      texhash
      )

    string (TOUPPER ${_latexExecutable} _latexExecutableVar)
    
    find_program (${_latexExecutableVar}_EXECUTABLE ${_latexExecutable}
      HINTS ${LATEX_ROOT_DIR}
      PATHS ${DAL_FIND_PATHS}
      PATH_SUFFIXES bin
      )
    
  endforeach (_latexExecutable)
  
  ##_____________________________________________________________________________
  ## Check for package style files

  set (_latexTest ${PROJECT_BINARY_DIR}/TestLATEX.tex)
  
  foreach (_latexPackage  
      eurosym
      graphicx
      fncychap
      hyperref
      latexsym
      listings
      makeidx
      )

    message (STATUS "Checking for LaTeX package ${_latexPackage}")

    string (TOUPPER ${_latexPackage} _latexPackageVar)
    
    ## Generate LaTeX source file
    file (WRITE  ${_latexTest} "\\documentclass[a4paper,fontsize=10pt]{scrartcl}"\n )
    file (APPEND ${_latexTest} "\\usepackage{"${_latexPackage}}\n )
    file (APPEND ${_latexTest} "\\begin{document}"\n )
    file (APPEND ${_latexTest} "This is a simple test document."\n )
    file (APPEND ${_latexTest} "\\end{document}"\n )

    ## Run the test file through LaTeX
    
    execute_process(
      COMMAND ${LATEX_EXECUTABLE} ${_latexTest}
      TIMEOUT 5
      WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
      RESULT_VARIABLE _latexTestResult
      OUTPUT_VARIABLE _latexTestOutput
      ERROR_VARIABLE _latexTestError
      )
    
    if (_latexTestResult)
      set (LATEX_${_latexPackageVar}_PACKAGE FALSE)
      message (STATUS "Checking for LaTeX package ${_latexPackage} - FAIL")
    else (_latexTestResult)
      set (LATEX_${_latexPackageVar}_PACKAGE TRUE)
      message (STATUS "Checking for LaTeX package ${_latexPackage} - Success")
    endif (_latexTestResult)
    
  endforeach (_latexPackage)
  
  ##_____________________________________________________________________________
  ## Actions taken when all components have been found
  
  if (LATEX_EXECUTABLE)
    set (LATEX_FOUND TRUE)
  else (LATEX_EXECUTABLE)
    set (LATEX_FOUND FALSE)
  endif (LATEX_EXECUTABLE)
  
  if (LATEX_FOUND)
    if (NOT LATEX_FIND_QUIETLY)
      message (STATUS "Found components for LATEX")
      message (STATUS "LATEX_ROOT_DIR   = ${LATEX_ROOT_DIR}")
      message (STATUS "LATEX_EXECUTABLE = ${LATEX_EXECUTABLE}")
      message (STATUS "DVIPS_EXECUTABLE = ${DVIPS_EXECUTABLE}")
    endif (NOT LATEX_FIND_QUIETLY)
  else (LATEX_FOUND)
    if (LATEX_FIND_REQUIRED)
      message (FATAL_ERROR "Could not find LATEX!")
    endif (LATEX_FIND_REQUIRED)
  endif (LATEX_FOUND)
  
  ##_____________________________________________________________________________
  ## Mark advanced variables
  
  mark_as_advanced (
    LATEX_ROOT_DIR
    LATEX_EXECUTABLE
    DVIPS_EXECUTABLE
    PDFLATEX_EXECUTABLE
    )
  
endif (NOT LATEX_FOUND)
