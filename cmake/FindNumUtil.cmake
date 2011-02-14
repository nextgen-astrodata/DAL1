# +-----------------------------------------------------------------------------+
# | $Id::                                                                     $ |
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

# - Check for the presence of NUMUTIL
#
# The following variables are set when NUMUTIL is found:
#  NUMUTIL_FOUND     = Set to true, if all components of NUMUTIL have been found.
#  NUMUTIL_INCLUDES  = Include path for the header files of NUMUTIL
#  NUMUTIL_LIBRARIES = Link these to use NUMUTIL
#  NUMUTIL_LFLAGS    = Linker flags (optional)

if (NOT NUMUTIL_FOUND)
    
  ##_____________________________________________________________________________
  ## Check for the header files
  
  find_path (NUMUTIL_INCLUDES num_util.h
    PATHS /sw /usr /usr/local /opt/local ${CMAKE_INSTALL_PREFIX}
    PATH_SUFFIXES include include/num_util include/python/num_util
    )
  
  ##_____________________________________________________________________________
  ## Check for the library
  
  find_library (NUMUTIL_LIBRARIES num_util
    PATHS /sw /usr /usr/local /opt/local ${CMAKE_INSTALL_PREFIX}
    PATH_SUFFIXES lib lib/python
    )
  
  ##_____________________________________________________________________________
  ## Check for the executable
  
#  find_program (NUMUTIL_EXECUTABLE <package name>
#    PATHS /sw /usr /usr/local /opt/local ${CMAKE_INSTALL_PREFIX}
#    PATH_SUFFIXES bin
#    )
  
  ##_____________________________________________________________________________
  ## Actions taken when all components have been found
  
  if (NUMUTIL_INCLUDES AND NUMUTIL_LIBRARIES)
    set (NUMUTIL_FOUND TRUE)
  else (NUMUTIL_INCLUDES AND NUMUTIL_LIBRARIES)
    set (NUMUTIL_FOUND FALSE)
    if (NOT NUMUTIL_FIND_QUIETLY)
      if (NOT NUMUTIL_INCLUDES)
	message (STATUS "Unable to find NUMUTIL header files!")
      endif (NOT NUMUTIL_INCLUDES)
      if (NOT NUMUTIL_LIBRARIES)
	message (STATUS "Unable to find NUMUTIL library files!")
      endif (NOT NUMUTIL_LIBRARIES)
    endif (NOT NUMUTIL_FIND_QUIETLY)
  endif (NUMUTIL_INCLUDES AND NUMUTIL_LIBRARIES)
  
  if (NUMUTIL_FOUND)
    if (NOT NUMUTIL_FIND_QUIETLY)
      message (STATUS "Found components for NUMUTIL")
      message (STATUS "NUMUTIL_INCLUDES  = ${NUMUTIL_INCLUDES}")
      message (STATUS "NUMUTIL_LIBRARIES = ${NUMUTIL_LIBRARIES}")
    endif (NOT NUMUTIL_FIND_QUIETLY)
  else (NUMUTIL_FOUND)
    if (NUMUTIL_FIND_REQUIRED)
      message (FATAL_ERROR "Could not find NUMUTIL!")
    endif (NUMUTIL_FIND_REQUIRED)
  endif (NUMUTIL_FOUND)
  
  ##_____________________________________________________________________________
  ## Mark advanced variables
  
  mark_as_advanced (
    NUMUTIL_INCLUDES
    NUMUTIL_LIBRARIES
    )
  
endif (NOT NUMUTIL_FOUND)
