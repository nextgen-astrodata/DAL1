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

  ## Initialize variables
  
  set (NUMPY_FOUND     FALSE )
  set (NUMPY_INCLUDES  ""    )
  set (NUMPY_LIBRARIES ""    )

  ## Check for Python

  if (NOT PYTHON_FOUND)
    set (PYTHON_FIND_QUIETLY ${NUMPY_FIND_QUIETLY})
    include (FindPython_DAL)
  endif (NOT PYTHON_FOUND)

  if (PYTHON_SITE_PACKAGES_DIR)
    set (NUMPY_ROOT_DIR ${PYTHON_SITE_PACKAGES_DIR})
  endif ()

  if (NOT NUMPY_ROOT_DIR)
    set (NUMPY_ROOT_DIR ${CMAKE_INSTALL_PREFIX})
  endif (NOT NUMPY_ROOT_DIR)

  ##_____________________________________________________________________________
  ## Check for the header files
  
  find_path (NUMPY_NUMPYCONFIG_H numpy/_numpyconfig.h
    HINTS ${NUMPY_ROOT_DIR}
    PATHS ${DAL_FIND_PATHS}
    PATH_SUFFIXES
    include
    python/include
    python/core/include
    python/numpy/core/include
    numpy/core/include
    site-packages/numpy/core/include
    NO_DEFAULT_PATH
    )

  if (NUMPY_NUMPYCONFIG_H)
    list (APPEND NUMPY_INCLUDES ${NUMPY_NUMPYCONFIG_H})
    list (APPEND NUMPY_INCLUDES ${NUMPY_NUMPYCONFIG_H}/numpy)
  endif (NUMPY_NUMPYCONFIG_H)
  
  ##_____________________________________________________________________________
  ## Check for the library
  
  ## temporarily remove "lib" prefix when searching for the modules
  set (_numpyLibPrefixes ${CMAKE_FIND_LIBRARY_PREFIXES})
  set (CMAKE_FIND_LIBRARY_PREFIXES "")
  
  foreach (_numpyLibrary multiarray scalarmath)
    
    ## Convert library name to CMake variable
    string (TOUPPER ${_numpyLibrary} _numpy_var)
    
    ## Search for the library
    find_library (NUMPY_${_numpy_var}_LIBRARY ${_numpyLibrary}
      HINTS ${NUMPY_ROOT_DIR}
      PATHS ${DAL_FIND_PATHS}
      PATH_SUFFIXES
      lib
      numpy/core
      python/numpy/core
      lib/python/numpy/core
      )
    
    ## If library was found, add it to the list of libraries
    if (NUMPY_${_numpy_var}_LIBRARY)
      list (APPEND NUMPY_LIBRARIES ${NUMPY_${_numpy_var}_LIBRARY})
    endif (NUMPY_${_numpy_var}_LIBRARY)
    
  endforeach (_numpyLibrary)

  ## reinstate library prefixes
  set (CMAKE_FIND_LIBRARY_PREFIXES ${_numpyLibPrefixes})

  ##_____________________________________________________________________________
  ## If NUMPY_INCLUDES and NUMPY_LIBRARIES can not be found in the paths,
  ## use python interpreter itself to locate them
  
  if (NOT NUMPY_INCLUDES)
#      execute_process (
#	COMMAND ${PYTHON_EXECUTABLE}
#        ARGS "-c 'import numpy; print numpy.get_include()'"
#        OUTPUT_VARIABLE NUMPY_INCLUDES
#        RETURN_VALUE NUMPY_NOT_FOUND )
  
#      if (NUMPY_INCLUDE_DIR MATCHES "numpy")
#        set (NUMPY_FOUND TRUE)
#      else (NUMPY_INCLUDE_DIR MATCHES "numpy")
#        set (NUMPY_FOUND FALSE)
#      endif (NUMPY_INCLUDE_DIR MATCHES "numpy")
      
      if (NUMPY_INCLUDE_DIR MATCHES "Traceback")
      # Did not successfully include numpy
        set(NUMPY_FOUND FALSE)
      else (NUMPY_INCLUDE_DIR MATCHES "Traceback")
      # successful
        set (NUMPY_FOUND TRUE)
        set (NUMPY_INCLUDE_DIR ${NUMPY_INCLUDE_DIR} CACHE PATH "Numpy include path")
      endif (NUMPY_INCLUDE_DIR MATCHES "Traceback")
  
      if (NUMPY_FOUND)
        if (NOT NUMPY_FIND_QUIETLY)
          message (STATUS "Numpy headers found")
        endif (NOT NUMPY_FIND_QUIETLY)
      else (NUMPY_FOUND)
        if (NUMPY_FIND_REQUIRED)
          message (FATAL_ERROR "Numpy headers missing")
        endif (NUMPY_FIND_REQUIRED)
      endif (NUMPY_FOUND)
  
      mark_as_advanced (NUMPY_INCLUDE_DIR)
  endif (NOT NUMPY_INCLUDES)  
  
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
