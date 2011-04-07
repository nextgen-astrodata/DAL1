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
  ## Check for the header files
  
  find_path (LATEX_INCLUDES <header file(s)>
    HINTS ${LATEX_ROOT_DIR}
    PATHS ${DAL_FIND_PATHS}
    PATH_SUFFIXES include
    )
  
  ##_____________________________________________________________________________
  ## Check for the library
  
  find_library (LATEX_LIBRARIES <package name>
    HINTS ${LATEX_ROOT_DIR}
    PATHS ${DAL_FIND_PATHS}
    PATH_SUFFIXES lib
    )
  
  ##_____________________________________________________________________________
  ## Check for the executable
  
  find_program (LATEX_EXECUTABLE <package name>
    HINTS ${LATEX_ROOT_DIR}
    PATHS ${DAL_FIND_PATHS}
    PATH_SUFFIXES bin
    )
  
  ##_____________________________________________________________________________
  ## Actions taken when all components have been found
  
  if (LATEX_INCLUDES AND LATEX_LIBRARIES)
    set (LATEX_FOUND TRUE)
  else (LATEX_INCLUDES AND LATEX_LIBRARIES)
    set (LATEX_FOUND FALSE)
    if (NOT LATEX_FIND_QUIETLY)
      if (NOT LATEX_INCLUDES)
	message (STATUS "Unable to find LATEX header files!")
      endif (NOT LATEX_INCLUDES)
      if (NOT LATEX_LIBRARIES)
	message (STATUS "Unable to find LATEX library files!")
      endif (NOT LATEX_LIBRARIES)
    endif (NOT LATEX_FIND_QUIETLY)
  endif (LATEX_INCLUDES AND LATEX_LIBRARIES)
  
  if (LATEX_FOUND)
    if (NOT LATEX_FIND_QUIETLY)
      message (STATUS "Found components for LATEX")
      message (STATUS "LATEX_ROOT_DIR  = ${LATEX_ROOT_DIR}")
      message (STATUS "LATEX_INCLUDES  = ${LATEX_INCLUDES}")
      message (STATUS "LATEX_LIBRARIES = ${LATEX_LIBRARIES}")
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
    LATEX_INCLUDES
    LATEX_LIBRARIES
    )
  
endif (NOT LATEX_FOUND)
