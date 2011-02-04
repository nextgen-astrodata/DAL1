# +-----------------------------------------------------------------------------+
# | $Id::                                                                     $ |
# +-----------------------------------------------------------------------------+
# |   Copyright (C) 2011                                                        |
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

# - Check for the presence of GSL
#
# The following variables are set when GSL is found:
#  GSL_FOUND            = Set to true, if all components of GSL have been found.
#  GSL_INCLUDES         = Include path for the header files of GSL
#  GSL_GSL_LIBRARY      = libgsl
#  GSL_GSLCBLAS_LIBRARY = libgslcblas
#  GSL_LIBRARIES        = Link these to use GSL
#  GSL_LFLAGS           = Linker flags (optional)

if (NOT GSL_FOUND)
    
  ##_____________________________________________________________________________
  ## Check for the header files
  
  find_path (GSL_INCLUDES gsl_version.h gsl_sys.h gsl_nan.h
    PATHS /sw /usr /usr/local /opt/local ${CMAKE_INSTALL_PREFIX}
    PATH_SUFFIXES include include/gsl
    )
  
  ##_____________________________________________________________________________
  ## Check for the library

  set (GSL_LIBRARIES "")

  ## libgsl
  
  find_library (GSL_GSL_LIBRARY gsl
    PATHS /sw /usr /usr/local /opt/local ${CMAKE_INSTALL_PREFIX}
    PATH_SUFFIXES lib
    )
  
  if (GSL_GSL_LIBRARY)
    list (APPEND GSL_LIBRARIES ${GSL_GSL_LIBRARY})
  endif (GSL_GSL_LIBRARY)
  
  ## libgsl
  
  find_library (GSL_GSLCBLAS_LIBRARY gslcblas
    PATHS /sw /usr /usr/local /opt/local ${CMAKE_INSTALL_PREFIX}
    PATH_SUFFIXES lib lib/gsl
    )
  
  if (GSL_GSLCBLAS_LIBRARY)
    list (APPEND GSL_LIBRARIES ${GSL_GSLCBLAS_LIBRARY})
  endif (GSL_GSLCBLAS_LIBRARY)
  
  ##_____________________________________________________________________________
  ## Check for the executable
  
  find_program (GSL_CONFIG_EXECUTABLE gsl-config
    PATHS /sw /usr /usr/local /opt/local ${CMAKE_INSTALL_PREFIX}
    PATH_SUFFIXES bin bin/gsl
    )
  
  ##_____________________________________________________________________________
  ## Actions taken when all components have been found
  
  if (GSL_INCLUDES AND GSL_LIBRARIES)
    set (GSL_FOUND TRUE)
  else (GSL_INCLUDES AND GSL_LIBRARIES)
    set (GSL_FOUND FALSE)
    if (NOT GSL_FIND_QUIETLY)
      if (NOT GSL_INCLUDES)
	message (STATUS "Unable to find GSL header files!")
      endif (NOT GSL_INCLUDES)
      if (NOT GSL_LIBRARIES)
	message (STATUS "Unable to find GSL library files!")
      endif (NOT GSL_LIBRARIES)
    endif (NOT GSL_FIND_QUIETLY)
  endif (GSL_INCLUDES AND GSL_LIBRARIES)
  
  if (GSL_FOUND)
    if (NOT GSL_FIND_QUIETLY)
      message (STATUS "Found components for GSL")
      message (STATUS "GSL_INCLUDES  = ${GSL_INCLUDES}")
      message (STATUS "GSL_LIBRARIES = ${GSL_LIBRARIES}")
    endif (NOT GSL_FIND_QUIETLY)
  else (GSL_FOUND)
    if (GSL_FIND_REQUIRED)
      message (FATAL_ERROR "Could not find GSL!")
    endif (GSL_FIND_REQUIRED)
  endif (GSL_FOUND)
  
  ##_____________________________________________________________________________
  ## Mark advanced variables
  
  mark_as_advanced (
    GSL_INCLUDES
    GSL_LIBRARIES
    GSL_GSL_LIBRARY
    GSL_GSLCBLAS_LIBRARY
    GSL_CONFIG_EXECUTABLE
    )
  
endif (NOT GSL_FOUND)
