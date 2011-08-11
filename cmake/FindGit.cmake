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

# - Check for the presence of GIT
#
# The following variables are set when GIT is found:
#  GIT_FOUND      = Set to true, if all components of GIT have been found.
#  GIT_INCLUDES   = Include path for the header files of GIT
#  GIT_LIBRARIES  = Link these to use GIT
#  GIT_LFLAGS     = Linker flags (optional)

if (NOT GIT_FOUND)
    
  if (NOT GIT_ROOT_DIR)
    set (GIT_ROOT_DIR ${CMAKE_INSTALL_PREFIX})
  endif (NOT GIT_ROOT_DIR)

  set (GIT_VERSION       "" )
  set (GIT_REMOTE_ORIGIN "" )
  
  ##_____________________________________________________________________________
  ## Check for the executable
  
  find_program (GIT_EXECUTABLE git
    HINTS ${GIT_ROOT_DIR}
    PATH_SUFFIXES bin
    )
  
  ##_____________________________________________________________________________
  ## Determine Git version

  if (GIT_EXECUTABLE)

    execute_process (
      COMMAND ${GIT_EXECUTABLE} --version
      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
      OUTPUT_VARIABLE GIT_VERSION
      OUTPUT_STRIP_TRAILING_WHITESPACE
      )
    
    if (GIT_VERSION)
      string(REGEX REPLACE "git version " "" GIT_VERSION ${GIT_VERSION})
    endif (GIT_VERSION)
    
  endif (GIT_EXECUTABLE)
  
  ##_____________________________________________________________________________
  ## Determine ID for the remote origin of the working copy
  
  if (GIT_EXECUTABLE)
    
    execute_process (
      COMMAND ${GIT_EXECUTABLE} remote -v
      COMMAND grep github
      COMMAND grep push
      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
      OUTPUT_VARIABLE GIT_REMOTE_ORIGIN
      )
    
    if (GIT_REMOTE_ORIGIN)
      string(REGEX MATCH [a-z]* GIT_REMOTE_ORIGIN ${GIT_REMOTE_ORIGIN})
    endif (GIT_REMOTE_ORIGIN)
    
  endif (GIT_EXECUTABLE)
  
  ##_____________________________________________________________________________
  ## Actions taken when all components have been found
  
  if (GIT_EXECUTABLE)
    set (GIT_FOUND TRUE)
  else (GIT_EXECUTABLE)
    set (GIT_FOUND FALSE)
    if (NOT GIT_FIND_QUIETLY)
      message (STATUS "Unable to find GIT executable!")
    endif (NOT GIT_FIND_QUIETLY)
  endif (GIT_EXECUTABLE)
  
  if (GIT_FOUND)
    if (NOT GIT_FIND_QUIETLY)
      message (STATUS "Found components for GIT")
      message (STATUS "GIT_ROOT_DIR      = ${GIT_ROOT_DIR}")
      message (STATUS "GIT_EXECUTABLE    = ${GIT_EXECUTABLE}")
      message (STATUS "GIT_VERSION       = ${GIT_VERSION}")
      message (STATUS "GIT_REMOTE_ORIGIN = ${GIT_REMOTE_ORIGIN}")
    endif (NOT GIT_FIND_QUIETLY)
  else (GIT_FOUND)
    if (GIT_FIND_REQUIRED)
      message (FATAL_ERROR "Could not find GIT!")
    endif (GIT_FIND_REQUIRED)
  endif (GIT_FOUND)
  
  ##_____________________________________________________________________________
  ## Mark advanced variables
  
  mark_as_advanced (
    GIT_ROOT_DIR
    GIT_EXECUTABLE
    GIT_VERSION
    GIT_REMOTE_ORIGIN
    )
  
endif (NOT GIT_FOUND)
