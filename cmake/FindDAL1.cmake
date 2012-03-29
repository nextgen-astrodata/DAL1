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

# - Check for the presence of DAL1
#
# The following variables are set when DAL is found:
#
#   DAL_FOUND      = Set to true, if all components of DAL1 have been found.
#   DAL_INCLUDES   = Include path for the header files of DAL1
#   DAL_LIBRARIES  = Link these to use DAL1
#   DAL_LFLAGS     = Linker flags (optional)
#   DAL_VERSION_MAJOR = Major version number
#   DAL_VERSION_MINOR = Minor version number
#   DAL_VERSION_PATCH = Patch version number
#   DAL_VERSION       = DAL version string
#
# In order to maintain compatibility with the Filesystem Hierarchy Standard (FHS)
# the following default installation layout has been defined:
#
# /opt
# └── dal1                         DAL_INSTALL_PREFIX
#     ├── bin                      DAL_INSTALL_BINDIR
#     ├── include                  DAL_INSTALL_INCLUDEDIR
#     │   ├── coordinates
#     │   ├── core
#     │   ├── data_common
#     │   └── data_hl
#     ├── lib                      DAL_INSTALL_INCLUDEDIR
#     └── share                    DAL_INSTALL_DATAROOTDIR
#         └── doc                  DAL_INSTALL_DOCDIR
#             └── html
#

if (NOT DAL_FOUND)
    
  if (NOT DAL_ROOT_DIR)
    set (DAL_ROOT_DIR ${CMAKE_INSTALL_PREFIX})
  endif (NOT DAL_ROOT_DIR)
  
  ##_____________________________________________________________________________
  ## Check for the header files
  
  find_path (DAL_INCLUDES dal_config.h
    HINTS ${DAL_ROOT_DIR}
    PATH_SUFFIXES include include/dal1
    )
  
  ## core/dalDataset.h
  
  find_path (DAL_DALDATASET_H core/dalDataset.h
    HINTS ${DAL_ROOT_DIR}
    PATH_SUFFIXES include include/dal1
    )
  if (DAL_DALDATASET_H)
    list (APPEND DAL_INCLUDES ${DAL_DALDATASET_H})
  endif (DAL_DALDATASET_H)
  
  ## core/dalDataset.h
  
  find_path (DAL_COORDINATE_H coordinates/Coordinate.h
    HINTS ${DAL_ROOT_DIR}
    PATH_SUFFIXES include include/dal1
    )
  if (DAL_COORDINATE_H)
    list (APPEND DAL_INCLUDES ${DAL_COORDINATE_H})
  endif (DAL_COORDINATE_H)
  
  ## Remove duplicate entries

  if (DAL_INCLUDES)
    list (REMOVE_DUPLICATES DAL_INCLUDES)
  endif (DAL_INCLUDES)
  
  ##_____________________________________________________________________________
  ## Check for the library

  set (DAL_LIBRARIES "")
  
  find_library (DAL_DAL_LIBRARY dal1
    HINTS ${DAL_ROOT_DIR}
    PATH_SUFFIXES lib lib/dal1
    )
  if (DAL_DAL_LIBRARY)
    list (APPEND DAL_LIBRARIES ${DAL_DAL_LIBRARY})
  endif (DAL_DAL_LIBRARY)
  
  ##_____________________________________________________________________________
  ## Check for the executable(s)
  
  foreach (_dal_executable
      lopes2h5
      msread
      ms2h5
      tbb2h5
      TBBraw2h5
      )
    
    ## try to locate the executable
    find_program (DAL_${_dal_executable}_EXECUTABLE ${_dal_executable}
      HINTS ${DAL_ROOT_DIR}
      PATH_SUFFIXES bin bin/dal1
      )
    
  endforeach (_dal_executable)
  
  ##_____________________________________________________________________________
  ## Test DAL1 library for:
  ##  - library version <major.minor.release>
  ##  - registered external packages (e.g. casacore, HDF5, etc.)
  
  if (DAL_INCLUDES AND DAL_LIBRARIES)
    ## Locate test program
    find_file (HAVE_TestDALLibrary TestDALLibrary.cc
      PATHS ${PROJECT_SOURCE_DIR}
      PATH_SUFFIXES cmake devel_common/cmake Modules
      )
    ## Build and run test program
    if (HAVE_TestDALLibrary)
      try_run(DAL_VERSION_RUN_RESULT DAL_VERSION_COMPILE_RESULT
	${PROJECT_BINARY_DIR}
	${HAVE_TestDALLibrary}
#	CMAKE_FLAGS -DLINK_LIBRARIES:STRING=${DAL_LIBRARIES}
	COMPILE_DEFINITIONS -I${DAL_INCLUDES}
	RUN_OUTPUT_VARIABLE DAL_VERSION_OUTPUT
	)
    endif (HAVE_TestDALLibrary)
  endif (DAL_INCLUDES AND DAL_LIBRARIES)
  
  ##_____________________________________________________________________________
  ## Find library version

  if (DAL_INCLUDES AND EXISTS "${DAL_INCLUDES}/dal_config.h")
    file (STRINGS "${DAL_INCLUDES}/dal_config.h" DAL_H REGEX "^#define DAL1_VERSION \"[^\"]*\"$")

    string (REGEX REPLACE "^.*DAL1_VERSION \"([0-9]+).*$" "\\1" DAL_VERSION_MAJOR "${DAL_H}")
    string (REGEX REPLACE "^.*DAL1_VERSION \"[0-9]+\\.([0-9]+).*$" "\\1" DAL_VERSION_MINOR  "${DAL_H}")
    string (REGEX REPLACE "^.*DAL1_VERSION \"[0-9]+\\.[0-9]+\\.([0-9]+).*$" "\\1" DAL_VERSION_PATCH "${DAL_H}")
    set (DAL_VERSION "${DAL_VERSION_MAJOR}.${DAL_VERSION_MINOR}.${DAL_VERSION_PATCH}")

  else (DAL_INCLUDES AND EXISTS "${DAL_INCLUDES}/dal_config.h")
    message (ERROR "Cannot find dal_config.h file")
  endif (DAL_INCLUDES AND EXISTS "${DAL_INCLUDES}/dal_config.h")

  ##_____________________________________________________________________________
  ## Actions taken when all components have been found
  
  if (DAL_INCLUDES AND DAL_LIBRARIES)
    set (DAL_FOUND TRUE)
  else (DAL_INCLUDES AND DAL_LIBRARIES)
    set (DAL_FOUND FALSE)
    if (NOT DAL_FIND_QUIETLY)
      if (NOT DAL_INCLUDES)
	message (STATUS "Unable to find DAL1 header files!")
      endif (NOT DAL_INCLUDES)
      if (NOT DAL_LIBRARIES)
	message (STATUS "Unable to find DAL1 library files!")
      endif (NOT DAL_LIBRARIES)
    endif (NOT DAL_FIND_QUIETLY)
  endif (DAL_INCLUDES AND DAL_LIBRARIES)
  
  if (DAL_FOUND)
    if (NOT DAL_FIND_QUIETLY)
      message (STATUS "Found components for DAL")
      message (STATUS "DAL_ROOT_DIR  = ${DAL_ROOT_DIR}")
      message (STATUS "DAL_INCLUDES  = ${DAL_INCLUDES}")
      message (STATUS "DAL_LIBRARIES = ${DAL_LIBRARIES}")
    endif (NOT DAL_FIND_QUIETLY)
  else (DAL_FOUND)
    if (DAL_FIND_REQUIRED)
      message (FATAL_ERROR "Could not find DAL1!")
    endif (DAL_FIND_REQUIRED)
  endif (DAL_FOUND)
  
  ##_____________________________________________________________________________
  ## Mark advanced variables
  
  mark_as_advanced (
    DAL_ROOT_DIR
    DAL_INCLUDES
    DAL_LIBRARIES
    )
  
endif (NOT DAL_FOUND)
