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
      )

    string (TOUPPER ${_latexExecutable} _latexVar)
    
    find_program (${_latexVar}_EXECUTABLE ${_latexExecutable}
      HINTS ${LATEX_ROOT_DIR}
      PATHS ${DAL_FIND_PATHS}
      PATH_SUFFIXES bin
      )
    
  endforeach (_latexExecutable)
  
  ##_____________________________________________________________________________
  ## Check for package style files
  
#  fncychap.sty  

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
    )
  
endif (NOT LATEX_FOUND)
