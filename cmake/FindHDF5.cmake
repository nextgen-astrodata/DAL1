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

if (NOT HDF5_FOUND)
    
  ##_____________________________________________________________________________
  ## Check for the header files
  
  find_path (HDF5_INCLUDES hdf5.h hdf5_hl.h
    PATHS /sw /usr /usr/local /opt/local ${CMAKE_INSTALL_PREFIX}
    PATH_SUFFIXES include include/hdf5
    )
  
  find_path (HAVE_HDF5_HDF5_H hdf5.h
    PATHS /sw /usr /usr/local /opt/local ${CMAKE_INSTALL_PREFIX}
    PATH_SUFFIXES include include/hdf5
    )
  
  find_path (HAVE_HDF5_H5LT_H H5LT.h
    PATHS /sw /usr /usr/local /opt/local ${CMAKE_INSTALL_PREFIX}
    PATH_SUFFIXES include include/hdf5
    )
  
  find_path (HAVE_HDF5_HDF5_HL_H hdf5_hl.h
    PATHS /sw /usr /usr/local /opt/local ${CMAKE_INSTALL_PREFIX}
    PATH_SUFFIXES include include/hdf5
    )

  ##_____________________________________________________________________________
  ## Check for the library
  
  set (HDF5_LIBRARIES "")

  ## Core library (libhdf5)
  
  find_library (HDF5_HDF5_LIBRARY hdf5
    PATHS /sw /usr /usr/local /opt/local ${CMAKE_INSTALL_PREFIX}
    PATH_SUFFIXES lib hdf5/lib
    )
  
  if (HDF5_HDF5_LIBRARY)
    set (HDF5_LIBRARIES ${HDF5_HDF5_LIBRARY})
  endif (HDF5_HDF5_LIBRARY)
  
  ## High level interface (libhdf5_hl)
  
  FIND_LIBRARY (HDF5_HDF5_HL_LIBRARY hdf5_hl
    PATHS /sw /usr /usr/local /opt/local ${CMAKE_INSTALL_PREFIX}
    PATH_SUFFIXES lib hdf5/lib
    )
  
  if (HDF5_HDF5_HL_LIBRARY)
    list (APPEND HDF5_LIBRARIES ${HDF5_HDF5_HL_LIBRARY})
  endif (HDF5_HDF5_HL_LIBRARY)
  
  ## C++ interface (libhdf5_cpp)
  
  FIND_LIBRARY (HDF5_HDF5_CPP_LIBRARY hdf5_cpp
    PATHS /sw /usr /usr/local /opt/local ${CMAKE_INSTALL_PREFIX}
    PATH_SUFFIXES lib hdf5/lib
    )
  
  if (HDF5_HDF5_CPP_LIBRARY)
    list (APPEND HDF5_LIBRARIES ${HDF5_HDF5_CPP_LIBRARY})
  endif (HDF5_HDF5_CPP_LIBRARY)

  ##_____________________________________________________________________________
  ## Check for the executable
  
  find_program (H5CHECK_EXECUTABLE h5check
    PATHS /sw /usr /usr/local /opt/local ${CMAKE_INSTALL_PREFIX}
    PATH_SUFFIXES bin
    )
  
  find_program (H5DUMP_EXECUTABLE h5dump 
    PATHS /sw /usr /usr/local /opt/local ${CMAKE_INSTALL_PREFIX}
    PATH_SUFFIXES bin
    )
  
  find_program (H5LS_EXECUTABLE h5ls 
    PATHS /sw /usr /usr/local /opt/local ${CMAKE_INSTALL_PREFIX}
    PATH_SUFFIXES bin
    )
  
  find_program (H5STAT_EXECUTABLE h5stat 
    PATHS /sw /usr /usr/local /opt/local ${CMAKE_INSTALL_PREFIX}
    PATH_SUFFIXES bin
    )

  ##_____________________________________________________________________________
  ## Determine library version
  
  find_file (CHECK_LIBRARY_VERSION_CC TestHDF5LibraryVersion.cc
    PATHS ${PROJECT_SOURCE_DIR}
    PATH_SUFFIXES cmake Modules
    )
  
  if (CHECK_LIBRARY_VERSION_CC)

    ## Locate compression library required for linking
    FIND_LIBRARY (HDF5_ZLIB_LIBRARY z
      PATHS /sw /usr /usr/local /opt/local ${CMAKE_INSTALL_PREFIX}
      PATH_SUFFIXES lib hdf5/lib
      )

    ## try to compile and run the test program
    try_run(HDF5_VERSION_RUN_RESULT HDF5_VERSION_COMPILE_RESULT
      ${PROJECT_BINARY_DIR}
      ${CHECK_LIBRARY_VERSION_CC}
      CMAKE_FLAGS -DLINK_LIBRARIES:FILEPATH=${HDF5_HDF5_LIBRARY}
      COMPILE_DEFINITIONS -I${HAVE_HDF5_HDF5_H}
      OUTPUT_VARIABLE HDF5_VERSION_OUTPUT_VARIABLE
      )

    ## Feedback
    if (HDF5_VERSION_RUN_RESULT)
      message (STATUS "[HDF5] Run result      = ${HDF5_VERSION_RUN_RESULT}")
      message (STATUS "[HDF5] Compile result  = ${HDF5_VERSION_COMPILE_RESULT}")
      message (STATUS "[HDF5] Program output  = ${HDF5_VERSION_OUTPUT_VARIABLE}")
    else (HDF5_VERSION_RUN_RESULT)
      message (STATUS "[HDF5] Failed to determine HDF5 library version!")
    endif (HDF5_VERSION_RUN_RESULT)
    
  else (CHECK_LIBRARY_VERSION_CC)
    
    message (WARNING "[HDF5] Unable to find test program TestHDF5LibraryVersion.cc")
    set (HDF5_VERSION_MAJOR   "-1" )
    set (HDF5_VERSION_MINOR   "-1" )
    set (HDF5_VERSION_RELEASE "-1" )

  endif (CHECK_LIBRARY_VERSION_CC)
  
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
    endif (NOT HDF5_FIND_QUIETLY)
  else (HDF5_FOUND)
    if (HDF5_FIND_REQUIRED)
      message (FATAL_ERROR "Could not find HDF5!")
    endif (HDF5_FIND_REQUIRED)
  endif (HDF5_FOUND)
  
  ##_____________________________________________________________________________
  ## Mark advanced variables
  
  mark_as_advanced (
    HDF5_INCLUDES
    HDF5_LIBRARIES
    )
  
endif (NOT HDF5_FOUND)
