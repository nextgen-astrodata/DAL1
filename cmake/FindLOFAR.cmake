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

# - Check for the presence of LOFAR
#
# The following variables are set when LOFAR is found:
#  LOFAR_FOUND      = Set to true, if all components of LOFAR have been found.
#  LOFAR_INCLUDES   = Include path for the header files of LOFAR
#  LOFAR_LIBRARIES  = Link these to use LOFAR
#  LOFAR_LFLAGS     = Linker flags (optional)

if (NOT LOFAR_FOUND)
    
  set (LOFAR_FOUND FALSE)

  if (NOT LOFAR_ROOT_DIR)
    set (LOFAR_ROOT_DIR ${CMAKE_INSTALL_PREFIX})
  endif (NOT LOFAR_ROOT_DIR)

  ##_____________________________________________________________________________
  ## Check for the header files
  
  find_path (LOFAR_INCLUDES Interface/Parset.h
    HINTS ${LOFAR_ROOT_DIR}
    PATHS /sw /usr /usr/local /opt /opt/local ${CMAKE_INSTALL_PREFIX}
    PATH_SUFFIXES include lofar/include
    )

  ##_____________________________________________________________________________
  ## Check for the library
  
  set (LOFAR_LIBRARIES "")

  foreach (_lofar_lib common interface)
    
    ## Convert library name to CMake variable
    string (TOUPPER ${_lofar_lib} _lofar_var)

    ## Search for the library
    find_library (LOFAR_${_lofar_var}_LIBRARY ${_lofar_lib}
      HINTS ${LOFAR_ROOT_DIR}
      PATHS /sw /usr /usr/local /opt /opt/local ${CMAKE_INSTALL_PREFIX}
      PATH_SUFFIXES lib lofar/lib
      )
    
    ## If library was found, add it to the list of libraries
    if (LOFAR_${_lofar_var}_LIBRARY)
      list (APPEND LOFAR_LIBRARIES ${LOFAR_${_lofar_var}_LIBRARY})
    endif (LOFAR_${_lofar_var}_LIBRARY)
    
  endforeach (_lofar_lib)

  ##_____________________________________________________________________________
  ## Check for the executable
  
#  find_program (LOFAR_EXECUTABLE <package name>
#    PATHS /sw /usr /usr/local /opt/local ${CMAKE_INSTALL_PREFIX}
#    PATH_SUFFIXES bin
#    )
  
  ##_____________________________________________________________________________
  ## Actions taken when all components have been found
  
  if (LOFAR_INCLUDES AND LOFAR_LIBRARIES)
    set (LOFAR_FOUND TRUE)
  else (LOFAR_INCLUDES AND LOFAR_LIBRARIES)
    set (LOFAR_FOUND FALSE)
    if (NOT LOFAR_FIND_QUIETLY)
      if (NOT LOFAR_INCLUDES)
	message (STATUS "Unable to find LOFAR header files!")
      endif (NOT LOFAR_INCLUDES)
      if (NOT LOFAR_LIBRARIES)
	message (STATUS "Unable to find LOFAR library files!")
      endif (NOT LOFAR_LIBRARIES)
    endif (NOT LOFAR_FIND_QUIETLY)
  endif (LOFAR_INCLUDES AND LOFAR_LIBRARIES)
  
  if (LOFAR_FOUND)
    if (NOT LOFAR_FIND_QUIETLY)
      message (STATUS "Found components for LOFAR")
      message (STATUS "LOFAR_ROOT_DIR  = ${LOFAR_ROOT_DIR}")
      message (STATUS "LOFAR_INCLUDES  = ${LOFAR_INCLUDES}")
      message (STATUS "LOFAR_LIBRARIES = ${LOFAR_LIBRARIES}")
    endif (NOT LOFAR_FIND_QUIETLY)
  else (LOFAR_FOUND)
    if (LOFAR_FIND_REQUIRED)
      message (FATAL_ERROR "Could not find LOFAR!")
    endif (LOFAR_FIND_REQUIRED)
  endif (LOFAR_FOUND)
  
  ##_____________________________________________________________________________
  ## Mark advanced variables
  
  mark_as_advanced (
    LOFAR_ROOT_DIR
    LOFAR_INCLUDES
    LOFAR_LIBRARIES
    LOFAR_COMMON_LIBRARY
    LOFAR_INTERFACE_LIBRARY
    )
  
endif (NOT LOFAR_FOUND)
