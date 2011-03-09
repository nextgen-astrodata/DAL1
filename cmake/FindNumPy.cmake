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

# - Check for the presence of NUMPY
#
# The following variables are set when NUMPY is found:
#  NUMPY_FOUND      = Set to true, if all components of NUMPY have been found.
#  NUMPY_INCLUDES   = Include path for the header files of NUMPY
#  NUMPY_LIBRARIES  = Link these to use NUMPY
#  NUMPY_LFLAGS     = Linker flags (optional)

if (NOT NUMPY_FOUND)
  
  set (NUMPY_FOUND FALSE)

  if (NOT NUMPY_ROOT_DIR)
    set (NUMPY_ROOT_DIR ${CMAKE_INSTALL_PREFIX})
  endif (NOT NUMPY_ROOT_DIR)
  
  ##_____________________________________________________________________________
  ## Check for the header files
  
  find_path (NUMPY_INCLUDES numpy/arrayobject.h numpy/ndarrayobject.h
    HINTS ${NUMPY_ROOT_DIR}
    PATHS /sw /usr /usr/local /opt/local ${CMAKE_INSTALL_PREFIX}
    PATH_SUFFIXES
    include
    python/include
    python/core/include
    python/numpy/core/include
    )
  
  ##_____________________________________________________________________________
  ## Check for the library
  
  set (NUMPY_LIBRARIES "")

  foreach (_numpy_lib multiarray scalarmath)
    
    ## Convert library name to CMake variable
    string (TOUPPER ${_numpy_lib} _numpy_var)

    ## Search for the library
    find_library (NUMPY_${_numpy_var}_LIBRARY ${_numpy_lib}
      HINTS ${NUMPY_ROOT_DIR}
      PATHS /sw /usr /usr/local /opt/local ${CMAKE_INSTALL_PREFIX}
      PATH_SUFFIXES
      lib
      lib/python/numpy/core
      )
    
    ## If library was found, add it to the list of libraries
    if (NUMPY_${_numpy_var}_LIBRARY)
      list (APPEND NUMPY_LIBRARIES ${NUMPY_${_numpy_var}_LIBRARY})
    endif (NUMPY_${_numpy_var}_LIBRARY)
    
  endforeach (_numpy_lib)
  
  ##_____________________________________________________________________________
  ## Check for the executable
  
#  find_program (NUMPY_EXECUTABLE <package name>
#    PATHS /sw /usr /usr/local /opt/local ${CMAKE_INSTALL_PREFIX}
#    PATH_SUFFIXES bin
#    )
  
  ##_____________________________________________________________________________
  ## Actions taken when all components have been found
  
  if (NUMPY_INCLUDES AND NUMPY_LIBRARIES)
    set (NUMPY_FOUND TRUE)
  else (NUMPY_INCLUDES AND NUMPY_LIBRARIES)
    set (NUMPY_FOUND FALSE)
    if (NOT NUMPY_FIND_QUIETLY)
      if (NOT NUMPY_INCLUDES)
	message (STATUS "Unable to find NUMPY header files!")
      endif (NOT NUMPY_INCLUDES)
      if (NOT NUMPY_LIBRARIES)
	message (STATUS "Unable to find NUMPY library files!")
      endif (NOT NUMPY_LIBRARIES)
    endif (NOT NUMPY_FIND_QUIETLY)
  endif (NUMPY_INCLUDES AND NUMPY_LIBRARIES)
  
  if (NUMPY_FOUND)
    if (NOT NUMPY_FIND_QUIETLY)
      message (STATUS "Found components for NUMPY")
      message (STATUS "NUMPY_INCLUDES  = ${NUMPY_INCLUDES}")
      message (STATUS "NUMPY_LIBRARIES = ${NUMPY_LIBRARIES}")
    endif (NOT NUMPY_FIND_QUIETLY)
  else (NUMPY_FOUND)
    if (NUMPY_FIND_REQUIRED)
      message (FATAL_ERROR "Could not find NUMPY!")
    endif (NUMPY_FIND_REQUIRED)
  endif (NUMPY_FOUND)
  
  ##_____________________________________________________________________________
  ## Mark advanced variables
  
  mark_as_advanced (
    NUMPY_ROOT_DIR
    NUMPY_INCLUDES
    NUMPY_LIBRARIES
    )
  
endif (NOT NUMPY_FOUND)
