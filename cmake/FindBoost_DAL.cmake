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

# - Check for the presence of BOOST
#
# The following variables are set when BOOST is found:
#  BOOST_FOUND         = Set to true, if all components of BOOST have been found.
#  BOOST_INCLUDES      = Include path for the header files of BOOST
#  BOOST_LIBRARIES     = Link these to use BOOST
#  BOOST_VERSION       = Boost library version <major>.<minor>.<patch>
#  BOOST_VERSION_MAJOR = Boost library major version.
#  BOOST_VERSION_MINOR = Boost library minor version.
#  BOOST_VERSION_PATCH = Boost library patch version.

if (NOT BOOST_FOUND)

  ## Initialization: search locations ______________________

  if (NOT BOOST_ROOT_DIR)
    set (BOOST_ROOT_DIR ${CMAKE_INSTALL_PREFIX})
  endif (NOT BOOST_ROOT_DIR)

  ## Locations where to search for Boost
  list (APPEND BOOST_ROOT_DIR
    /opt
    /opt/local
    /opt/casa/local
    /sw
    /usr
    /usr/local
    /app/usg
    ${CMAKE_INSTALL_PREFIX}
    )
  
  ## Exclude automatic searching of system paths
  set(Boost_NO_SYSTEM_PATHS TRUE)
  
  ## Initialization: Version and multithreading ____________
  
  set (BOOST_VERSION_MAJOR  0      )
  set (BOOST_VERSION_MINOR  0      )
  set (BOOST_VERSION_PATCH  0      )
  set (Boost_BASE_VERSION   "1.40" )

  set (Boost_NO_SYSTEM_PATHS    YES )
  set (Boost_USE_MULTITHREADED  YES )
  set (Boost_USE_STATIC_RUNTIME YES )
  
  if (APPLE)
    set (Boost_USE_STATIC_LIBS YES )
  else (APPLE)
    set (Boost_USE_STATIC_LIBS NO )
  endif (APPLE)
  
  ## Enable/diable verbosity mode __________________________
  
  if (DAL_VERBOSE_CONFIGURE)
    set (Boost_FIND_QUIETLY  NO  )
  else (DAL_VERBOSE_CONFIGURE)
    set (Boost_FIND_QUIETLY  YES )
  endif (DAL_VERBOSE_CONFIGURE)
  
  ## Initialization: Boost modules to search for ___________

  set (BOOST_MODULES
    date_time
    program_options
    python
    thread
    )
  
  ##_____________________________________________________________________________
  ## Check for header files and libraries
  
  foreach (BOOST_ROOT ${BOOST_ROOT_DIR})

    ## Call to the CMake's standard FindBoost.cmake module
    find_package (Boost ${Boost_BASE_VERSION} COMPONENTS ${BOOST_MODULES})
    
    if (Boost_FOUND)
      ## Map basic variables ...
      set (HAVE_BOOST       ${Boost_FOUND}        )
      set (BOOST_FOUND      ${Boost_FOUND}        )
      set (BOOST_LIBRARIES  ${Boost_LIBRARIES}    )
      set (BOOST_INCLUDES   ${Boost_INCLUDE_DIRS} )
      ## ... and exit the search
      break ()
    endif (Boost_FOUND)

    ## reset variables

    unset (Boost_FOUND        CACHE)
    unset (Boost_LIBRARIES    CACHE)
    unset (Boost_INCLUDE_DIRS CACHE)

    foreach (_boostModuleLibrary ${BOOST_MODULES})
      string (TOUPPER ${_boostModuleLibrary} _boostModuleLibraryVar)
      unset (Boost_${_boostModuleLibraryVar}_LIBRARY CACHE)
    endforeach ()

  endforeach (BOOST_ROOT)

  ## Additional check in case the header files were not found by the standard module

  if (BOOST_LIBRARIES AND NOT BOOST_INCLUDES)
    find_path (BOOST_INCLUDES boost/program_options.hpp boost/thread.hpp
      HINTS ${BOOST_ROOT_DIR}
      PATHS ${DAL_FIND_PATHS}
      PATH_SUFFIXES include
      )
  endif (BOOST_LIBRARIES AND NOT BOOST_INCLUDES)
  
  ## Clean up the list of include directories
  if (BOOST_INCLUDES)
    list (REMOVE_DUPLICATES BOOST_INCLUDES)
  endif (BOOST_INCLUDES)
  
  ##_____________________________________________________________________________
  ## Actions taken when all components have been found
  
  if (BOOST_INCLUDES AND BOOST_LIBRARIES)
    ## Set library found
    set (BOOST_FOUND TRUE)
    ## Get library version and configuration
    include (TestBoost)
  else (BOOST_INCLUDES AND BOOST_LIBRARIES)
    set (BOOST_FOUND FALSE)
    if (NOT BOOST_FIND_QUIETLY)
      if (NOT BOOST_INCLUDES)
	message (STATUS "Unable to find BOOST header files!")
      endif (NOT BOOST_INCLUDES)
      if (NOT BOOST_LIBRARIES)
	message (STATUS "Unable to find BOOST library files!")
      endif (NOT BOOST_LIBRARIES)
    endif (NOT BOOST_FIND_QUIETLY)
  endif (BOOST_INCLUDES AND BOOST_LIBRARIES)
  
  if (BOOST_FOUND)
    if (NOT BOOST_FIND_QUIETLY)
      message (STATUS "Found components for BOOST")
      message (STATUS "BOOST_ROOT_DIR  = ${BOOST_ROOT_DIR}"  )
      message (STATUS "BOOST_VERSION   = ${BOOST_VERSION}"   )
      message (STATUS "BOOST_INCLUDES  = ${BOOST_INCLUDES}"  )
      message (STATUS "BOOST_LIBRARIES = ${BOOST_LIBRARIES}" )
    endif (NOT BOOST_FIND_QUIETLY)
  else (BOOST_FOUND)
    if (BOOST_FIND_REQUIRED)
      message (FATAL_ERROR "Could not find BOOST!")
    endif (BOOST_FIND_REQUIRED)
  endif (BOOST_FOUND)
  
  ##_____________________________________________________________________________
  ## Mark advanced variables
  
  mark_as_advanced (
    BOOST_ROOT_DIR
    BOOST_VERSION
    BOOST_INCLUDES
    BOOST_LIBRARIES
    )
  
endif (NOT BOOST_FOUND)
