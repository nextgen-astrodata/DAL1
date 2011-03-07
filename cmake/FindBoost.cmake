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

  ## Initialization of variables

  if (NOT BOOST_ROOT_DIR)
    set (BOOST_ROOT_DIR ${CMAKE_INSTALL_PREFIX})
  endif (NOT BOOST_ROOT_DIR)

  set (BOOST_VERSION_MAJOR  0     )
  set (BOOST_VERSION_MINOR  0     )
  set (BOOST_VERSION_PATCH  0     )
  set (BOOST_WITH_THREAD    FALSE )
  
  set (BOOST_MODULES
    date_time
    filesystem
#    graph
    iostreams
    math_c99
    math_c99f
    math_c99l
    math_tr1
    math_tr1f
    math_tr1l
    prg_exec_monitor
    program_options
    python
    random
    regex
    serialization
    signals
    system
    test_exec_monitor
    thread
    unit_test_framework
    wave
    wserialization
    )
  
  ##_____________________________________________________________________________
  ## Check for header files and libraries
  
  set (BOOST_INCLUDES "")
  set (BOOST_LIBRARIES "")

  foreach (_module ${BOOST_MODULES})
    
    ## Convert library name to CMake variable
    string (TOUPPER ${_module} _boost_var)

    ## Search for the include directory
    find_path (BOOST_${_boost_var}_INCLUDES boost/${_module}.hpp
      HINTS ${BOOST_ROOT_DIR}
      PATHS /sw /usr /usr/local /opt/local
      PATH_SUFFIXES include include/boost
      )
    if (BOOST_${_boost_var}_INCLUDES)
      list (APPEND BOOST_INCLUDES ${BOOST_${_boost_var}_INCLUDES})
    endif (BOOST_${_boost_var}_INCLUDES)
    
    ## Search for the library
    find_library (BOOST_${_boost_var}_LIBRARY boost_${_module}-mt boost_${_module}
      HINTS ${BOOST_ROOT_DIR}
      PATHS /sw /usr /usr/local /opt/local
      PATH_SUFFIXES lib
      )
    if (BOOST_${_boost_var}_LIBRARY)
      list (APPEND BOOST_LIBRARIES ${BOOST_${_boost_var}_LIBRARY})
    endif (BOOST_${_boost_var}_LIBRARY)
    
  endforeach (_module)

  ## Clean up the list of include directories
  list (REMOVE_DUPLICATES BOOST_INCLUDES)
  
  ##_____________________________________________________________________________
  ## Test Boost library for:
  ##  - library version <major.minor.release>
  ##  - usage of thread library
  
  ## Locate test program
  find_file (HAVE_TestBoostLibrary TestBoostLibrary.cc
    PATHS ${PROJECT_SOURCE_DIR}
    PATH_SUFFIXES cmake Modules
    )
  
  if (BOOST_INCLUDES AND BOOST_LIBRARIES AND HAVE_TestBoostLibrary)
    try_run(BOOST_VERSION_RUN_RESULT BOOST_VERSION_COMPILE_RESULT
      ${PROJECT_BINARY_DIR}
      ${HAVE_TestBoostLibrary}
      COMPILE_DEFINITIONS -I${BOOST_INCLUDES}
      RUN_OUTPUT_VARIABLE BOOST_VERSION_OUTPUT
      )
  endif (BOOST_INCLUDES AND BOOST_LIBRARIES AND HAVE_TestBoostLibrary)
  
  ## Comile of test program successful?
  if (BOOST_VERSION_COMPILE_RESULT)
    ## Run of test program successful?
    if (BOOST_VERSION_RUN_RESULT AND BOOST_VERSION_OUTPUT)
      
      ## Library version _________________________
      
      string(REGEX REPLACE "BOOST_VERSION ([0-9]+).*" "\\1" BOOST_VERSION ${BOOST_VERSION_OUTPUT})
      string(REGEX REPLACE ".*BOOST_VERSION_MAJOR ([0-9]+).*" "\\1" BOOST_VERSION_MAJOR ${BOOST_VERSION_OUTPUT})
      string(REGEX REPLACE ".*BOOST_VERSION_MINOR ([0-9]+).*" "\\1" BOOST_VERSION_MINOR ${BOOST_VERSION_OUTPUT})
      string(REGEX REPLACE ".*BOOST_VERSION_PATCH ([0-9]+).*" "\\1" BOOST_VERSION_PATCH ${BOOST_VERSION_OUTPUT})

    else (BOOST_VERSION_RUN_RESULT AND BOOST_VERSION_OUTPUT)
      message (STATUS "[Boost] Failed to run TestBoostLibrary!")
    endif (BOOST_VERSION_RUN_RESULT AND BOOST_VERSION_OUTPUT)
  else (BOOST_VERSION_COMPILE_RESULT)
    message (STATUS "[Boost] Failed to compile TestBoostLibrary!")
  endif (BOOST_VERSION_COMPILE_RESULT)

  ## Assemble full version of library
  set (BOOST_VERSION "${BOOST_VERSION_MAJOR}.${BOOST_VERSION_MINOR}.${BOOST_VERSION_PATCH}")

  ## Test support of thread library
  
  if (BOOST_INCLUDES AND BOOST_THREAD_LIBRARY AND HAVE_TestBoostLibrary)
    try_run(BOOST_THREAD_RUN_RESULT BOOST_THREAD_COMPILE_RESULT
      ${PROJECT_BINARY_DIR}
      ${HAVE_TestBoostLibrary}
      CMAKE_FLAGS -DLINK_LIBRARIES:STRING=${BOOST_THREAD_LIBRARY}
      COMPILE_DEFINITIONS -I${BOOST_INCLUDES} -DTEST_BOOST_THREAD
      COMPILE_OUTPUT_VARIABLE BOOST_THREAD_COMPILE_OUTPUT
      RUN_OUTPUT_VARIABLE BOOST_THREAD_RUN_OUTPUT
      )
  endif (BOOST_INCLUDES AND BOOST_THREAD_LIBRARY AND HAVE_TestBoostLibrary)
  
  if (BOOST_THREAD_COMPILE_RESULT)
    if (BOOST_THREAD_RUN_RESULT)
      message (STATUS "[Boost] Thread support enabled.")
      set (BOOST_WITH_THREAD YES)
    else (BOOST_THREAD_RUN_RESULT)
      set (BOOST_WITH_THREAD NO)
      message (STATUS "[Boost] Failed to run TestBoostLibrary with thread support!")
    endif (BOOST_THREAD_RUN_RESULT)
  else (BOOST_THREAD_COMPILE_RESULT)
    set (BOOST_WITH_THREAD NO)
    message (STATUS "[Boost] Failed to compile TestBoostLibrary with thread support!")
    message (${BOOST_THREAD_COMPILE_OUTPUT})
  endif (BOOST_THREAD_COMPILE_RESULT)
  
  ##_____________________________________________________________________________
  ## Actions taken when all components have been found
  
  if (BOOST_INCLUDES AND BOOST_LIBRARIES)
    set (BOOST_FOUND TRUE)
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
