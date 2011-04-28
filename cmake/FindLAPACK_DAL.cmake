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

# - Check for the presence of LAPACK
#
# The following variables are set when LAPACK is found:
#  LAPACK_FOUND      = Set to true, if all components of LAPACK have been found.
#  LAPACK_INCLUDES   = Include path for the header files of LAPACK
#  LAPACK_LIBRARIES  = Link these to use LAPACK
#  LAPACK_LFLAGS     = Linker flags (optional)

if (NOT LAPACK_FOUND)
    
  if (NOT LAPACK_ROOT_DIR)
    set (LAPACK_ROOT_DIR ${CMAKE_INSTALL_PREFIX})
  endif (NOT LAPACK_ROOT_DIR)
  
  ##_____________________________________________________________________________
  ## Check for the header files
  
  set (LAPACK_INCLUDES "")

  find_path (LAPACK_CLAPACK_H clapack.h
    HINTS ${LAPACK_ROOT_DIR}
    PATHS /sw /usr /usr/local /opt /opt/local ${CMAKE_INSTALL_PREFIX}
    PATH_SUFFIXES include
    )

  if (LAPACK_CLAPACK_H)
    list (APPEND LAPACK_INCLUDES ${LAPACK_CLAPACK_H})
  endif (LAPACK_CLAPACK_H)
  
  ##_____________________________________________________________________________
  ## Check for the libraries (lapack cblas blas atlas)
  
  set (LAPACK_LIBRARIES "")

  foreach (_lapack_lib lapack cblas blas atlas)
    
    ## Convert library name to CMake variable
    string (TOUPPER ${_lapack_lib} _lapack_var)

    ## Search for the library
    find_library (LAPACK_${_lapack_var}_LIBRARY ${_lapack_lib}
      HINTS ${LAPACK_ROOT_DIR}
      PATHS /sw /usr /usr/local /opt /opt/local ${CMAKE_INSTALL_PREFIX}
      PATH_SUFFIXES lib
      )
    
    ## If library was found, add it to the list of libraries
    if (LAPACK_${_lapack_var}_LIBRARY)
      list (APPEND LAPACK_LIBRARIES ${LAPACK_${_lapack_var}_LIBRARY})
    endif (LAPACK_${_lapack_var}_LIBRARY)
    
  endforeach (_lapack_lib)
  
  ##_____________________________________________________________________________
  ## Check for the executable
  
#  find_program (LAPACK_EXECUTABLE <package name>
#    HINTS ${LAPACK_ROOT_DIR}
#    PATHS /sw /usr /usr/local /opt /opt/local ${CMAKE_INSTALL_PREFIX}
#    PATH_SUFFIXES bin
#    )
  
  ##_____________________________________________________________________________
  ## Actions taken when all components have been found
  
  if (LAPACK_INCLUDES AND LAPACK_LIBRARIES)
    set (LAPACK_FOUND TRUE)
  else (LAPACK_INCLUDES AND LAPACK_LIBRARIES)
    set (LAPACK_FOUND FALSE)
    if (NOT LAPACK_FIND_QUIETLY)
      if (NOT LAPACK_INCLUDES)
	message (STATUS "Unable to find LAPACK header files!")
      endif (NOT LAPACK_INCLUDES)
      if (NOT LAPACK_LIBRARIES)
	message (STATUS "Unable to find LAPACK library files!")
      endif (NOT LAPACK_LIBRARIES)
    endif (NOT LAPACK_FIND_QUIETLY)
  endif (LAPACK_INCLUDES AND LAPACK_LIBRARIES)
  
  if (LAPACK_FOUND)
    if (NOT LAPACK_FIND_QUIETLY)
      message (STATUS "Found components for LAPACK")
      message (STATUS "LAPACK_INCLUDES  = ${LAPACK_INCLUDES}")
      message (STATUS "LAPACK_LIBRARIES = ${LAPACK_LIBRARIES}")
    endif (NOT LAPACK_FIND_QUIETLY)
  else (LAPACK_FOUND)
    if (LAPACK_FIND_REQUIRED)
      message (FATAL_ERROR "Could not find LAPACK!")
    endif (LAPACK_FIND_REQUIRED)
  endif (LAPACK_FOUND)
  
  ##_____________________________________________________________________________
  ## Mark advanced variables
  
  mark_as_advanced (
    LAPACK_ROOT_DIR
    LAPACK_INCLUDES
    LAPACK_LIBRARIES
    )
  
endif (NOT LAPACK_FOUND)
