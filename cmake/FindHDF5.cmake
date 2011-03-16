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

# - Check for the presence of HDF5
#
# The following variables are set when HDF5 is found:
#  HDF5_FOUND           = Set to true, if HDF5 header files and libraries have
#                         been found; if a specific component is required - such
#                         as e.g. the high-level interface - this should be
#                         checked for in the CMake configuration script.
#  HDF5_INCLUDES        = Include path for the header files of HDF5
#  HAVE_HDF5_HDF5_H     = Include path for the hdf5.h header file
#  HAVE_HDF5_H5LT_H     = Include path for the H5LT.h header file
#  HAVE_HDF5_HDF5_HL_H  = Include path for the hdf5_hl.h header file
#  HDF5_LIBRARIES       = Link these to use HDF5
#  HDF5_HDF5_LIBRARY    = libhdf5
#  HDF5_HDF5_HL_LIBRARY = libhdf5_hl
#  HDF5_LFLAGS          = Linker flags (optional)
#
# Further inspection of the characteristics of the located HDF5 library
# installation can be done based on the entries and definitions in H5pubconf.h:
#
#  H5_PACKAGE_VERSION    - Define to the version of this package.
#  H5_USE_16_API_DEFAULT - Define using v1.6 public API symbols by default.
#

if (NOT HDF5_FOUND)
  
  if (NOT HDF5_ROOT_DIR)
    set (HDF5_ROOT_DIR ${CMAKE_INSTALL_PREFIX})
  endif (NOT HDF5_ROOT_DIR)

  set (HDF5_VERSION_MAJOR   0      )
  set (HDF5_VERSION_MINOR   0      )
  set (HDF5_VERSION_RELEASE 0      )
  set (HDF5_HAVE_PARALLEL_IO FALSE )
  
  ##_____________________________________________________________________________
  ## Check for the header files
  
  find_path (HDF5_INCLUDES hdf5.h hdf5_hl.h
    HINTS ${HDF5_ROOT_DIR} 
    PATHS /sw /usr /usr/local /opt /opt/local ${CMAKE_INSTALL_PREFIX}
    PATH_SUFFIXES include include/hdf5
    )
  
  find_path (HAVE_HDF5_HDF5_H hdf5.h
    HINTS ${HDF5_ROOT_DIR} 
    PATHS /sw /usr /usr/local /opt /opt/local ${CMAKE_INSTALL_PREFIX}
    PATH_SUFFIXES include include/hdf5
    )
  
  find_path (HAVE_HDF5_H5LT_H H5LT.h
    HINTS ${HDF5_ROOT_DIR} 
    PATHS /sw /usr /usr/local /opt /opt/local ${CMAKE_INSTALL_PREFIX}
    PATH_SUFFIXES include include/hdf5
    )
  
  find_path (HAVE_HDF5_HDF5_HL_H hdf5_hl.h
    HINTS ${HDF5_ROOT_DIR} 
    PATHS /sw /usr /usr/local /opt /opt/local ${CMAKE_INSTALL_PREFIX}
    PATH_SUFFIXES include include/hdf5
    )

  ##_____________________________________________________________________________
  ## Check for the library
  
  set (HDF5_LIBRARIES "")

  foreach (_libHDF5 hdf5 hdf5_hl hdf5_cpp z)

    ## Convert library name to CMake variable
    string (TOUPPER ${_libHDF5} _varHDF5)
    
    ## Search for the library
    find_library (HDF5_${_varHDF5}_LIBRARY
      NAMES ${_libHDF5}
      HINTS ${HDF5_ROOT_DIR} 
      PATHS /sw /usr /usr/local /opt /opt/local ${CMAKE_INSTALL_PREFIX}
      PATH_SUFFIXES lib hdf5/lib
      )
    
    ## Update library list
    if (HDF5_${_varHDF5}_LIBRARY)
      list (APPEND HDF5_LIBRARIES ${HDF5_${_varHDF5}_LIBRARY})
    endif (HDF5_${_varHDF5}_LIBRARY)

  endforeach (_libHDF5)

  ##_____________________________________________________________________________
  ## Check for the executables

  ## Compilers

  find_program (HDF5_C_COMPILER h5cc h5pcc
    HINTS ${HDF5_ROOT_DIR} 
    PATHS /sw /usr /usr/local /opt /opt/local ${CMAKE_INSTALL_PREFIX}
    PATH_SUFFIXES bin
    )
  
  find_program (HDF5_CXX_COMPILER h5c++ h5pc++
    HINTS ${HDF5_ROOT_DIR} 
    PATHS /sw /usr /usr/local /opt /opt/local ${CMAKE_INSTALL_PREFIX}
    PATH_SUFFIXES bin
    )
  
  ## HDF5 tools
  
  find_program (H5CHECK_EXECUTABLE h5check
    HINTS ${HDF5_ROOT_DIR} 
    PATHS /sw /usr /usr/local /opt /opt/local ${CMAKE_INSTALL_PREFIX}
    PATH_SUFFIXES bin
    )
  
  find_program (H5DUMP_EXECUTABLE h5dump 
    HINTS ${HDF5_ROOT_DIR} 
    PATHS /sw /usr /usr/local /opt /opt/local ${CMAKE_INSTALL_PREFIX}
    PATH_SUFFIXES bin
    )
  
  find_program (H5LS_EXECUTABLE h5ls 
    HINTS ${HDF5_ROOT_DIR} 
    PATHS /sw /usr /usr/local /opt /opt/local ${CMAKE_INSTALL_PREFIX}
    PATH_SUFFIXES bin
    )
  
  find_program (H5STAT_EXECUTABLE h5stat 
    HINTS ${HDF5_ROOT_DIR} 
    PATHS /sw /usr /usr/local /opt /opt/local ${CMAKE_INSTALL_PREFIX}
    PATH_SUFFIXES bin
    )

  ##_____________________________________________________________________________
  ## Test HDF5 library for:
  ##  - library version <major.minor.release>
  ##  - parallel I/O support
  ##  - default API version
  
  if (HDF5_INCLUDES AND HDF5_LIBRARIES)
    ## Locate test programs
    find_file (HAVE_TestHDF5Library TestHDF5Library.cc
      PATHS ${PROJECT_SOURCE_DIR}
      PATH_SUFFIXES cmake devel_common/cmake Modules
      )
    find_file (HAVE_TestHDF5API TestHDF5API.cc
      PATHS ${PROJECT_SOURCE_DIR}
      PATH_SUFFIXES cmake devel_common/cmake Modules
      )
    ## Build and run test program
    if (HAVE_TestHDF5Library)
      if (MPI_FOUND)
	try_run(HDF5_VERSION_RUN_RESULT HDF5_VERSION_COMPILE_RESULT
	  ${PROJECT_BINARY_DIR}
	  ${HAVE_TestHDF5Library}
	  CMAKE_FLAGS -DLINK_LIBRARIES:STRING=${HDF5_LIBRARIES};${MPI_LIBRARIES}
	  COMPILE_DEFINITIONS -I${HDF5_INCLUDES} -I${MPI_INCLUDE_PATH}
	  COMPILE_OUTPUT_VARIABLE HDF5_VERSION_COMPILE_OUTPUT
	  RUN_OUTPUT_VARIABLE HDF5_VERSION_OUTPUT
	  )
      else (MPI_FOUND)
	try_run(HDF5_VERSION_RUN_RESULT HDF5_VERSION_COMPILE_RESULT
	  ${PROJECT_BINARY_DIR}
	  ${HAVE_TestHDF5Library}
	  CMAKE_FLAGS -DLINK_LIBRARIES:STRING=${HDF5_LIBRARIES}
	  COMPILE_DEFINITIONS -I${HDF5_INCLUDES}
	  COMPILE_OUTPUT_VARIABLE HDF5_VERSION_COMPILE_OUTPUT
	  RUN_OUTPUT_VARIABLE HDF5_VERSION_OUTPUT
	  )
      endif (MPI_FOUND)
    endif (HAVE_TestHDF5Library)
  endif (HDF5_INCLUDES AND HDF5_LIBRARIES)
  
  ## Comile of test program successful?
  if (HDF5_VERSION_COMPILE_RESULT)
    ## Run of test program successful?
    if (HDF5_VERSION_RUN_RESULT)

      ## Library version _________________________

      string(REGEX REPLACE "H5_VERS_MAJOR ([0-9]+).*" "\\1" HDF5_VERSION_MAJOR ${HDF5_VERSION_OUTPUT})
      string(REGEX REPLACE ".*H5_VERS_MINOR ([0-9]+).*" "\\1" HDF5_VERSION_MINOR ${HDF5_VERSION_OUTPUT})
      string(REGEX REPLACE ".*H5_VERS_RELEASE ([0-9]+).*" "\\1" HDF5_VERSION_RELEASE ${HDF5_VERSION_OUTPUT})

      ## Support for parallel I/O? _______________

      string(REGEX REPLACE ".*H5_HAVE_PARALLEL ([0-9]+).*" "\\1" HDF5_HAVE_PARALLEL_IO ${HDF5_VERSION_OUTPUT})

      ## Default API version _____________________

      string(REGEX REPLACE ".*H5_USE_16_API_DEFAULT ([0-9]+).*" "\\1" HDF5_USE_16_API_DEFAULT ${HDF5_VERSION_OUTPUT})

    else (HDF5_VERSION_RUN_RESULT)
      message (STATUS "[HDF5] Failed to run TestHDF5Library!")
    endif (HDF5_VERSION_RUN_RESULT)
  else (HDF5_VERSION_COMPILE_RESULT)
    message (STATUS "[HDF5] Failed to compile TestHDF5Library!")
    if (DAL_VERBOSE_CONFIGURE)
      message (STATUS "${HDF5_VERSION_COMPILE_OUTPUT}")
    endif (DAL_VERBOSE_CONFIGURE)
  endif (HDF5_VERSION_COMPILE_RESULT)
  
  set (HDF5_VERSION "${HDF5_VERSION_MAJOR}.${HDF5_VERSION_MINOR}.${HDF5_VERSION_RELEASE}")

  ##_____________________________________________________________________________
  ## Actions taken when all components have been found
  
  if (HDF5_INCLUDES AND HDF5_LIBRARIES)
    set (HDF5_FOUND TRUE)
  else (HDF5_INCLUDES AND HDF5_LIBRARIES)
    set (HDF5_FOUND FALSE)
    if (NOT HDF5_FIND_QUIETLY)
      if (NOT HDF5_INCLUDES)
	message (STATUS "Unable to find HDF5 header files!")
      endif (NOT HDF5_INCLUDES)
      if (NOT HDF5_LIBRARIES)
	message (STATUS "Unable to find HDF5 library files!")
      endif (NOT HDF5_LIBRARIES)
    endif (NOT HDF5_FIND_QUIETLY)
  endif (HDF5_INCLUDES AND HDF5_LIBRARIES)
  
  if (HDF5_FOUND)
    if (NOT HDF5_FIND_QUIETLY)
      message (STATUS "Found components for HDF5")
      message (STATUS "HDF5_INCLUDES  = ${HDF5_INCLUDES}")
      message (STATUS "HDF5_LIBRARIES = ${HDF5_LIBRARIES}")
      message (STATUS "PARALLEL_IO    = ${HDF5_HAVE_PARALLEL_IO}")
    endif (NOT HDF5_FIND_QUIETLY)
  else (HDF5_FOUND)
    if (HDF5_FIND_REQUIRED)
      message (FATAL_ERROR "Could not find HDF5!")
    endif (HDF5_FIND_REQUIRED)
  endif (HDF5_FOUND)
  
  ##_____________________________________________________________________________
  ## Mark advanced variables
  
  mark_as_advanced (
    HDF5_ROOT_DIR
    HDF5_INCLUDES
    HDF5_LIBRARIES
    HDF5_HAVE_PARALLEL_IO
    HDF5_USE_16_API_DEFAULT
    )
  
endif (NOT HDF5_FOUND)
