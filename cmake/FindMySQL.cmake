# +-----------------------------------------------------------------------------+
# | $Id::                                                                     $ |
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

# - Check for the presence of MYSQL
#
# The following variables are set when MYSQL is found:
#  MYSQL_FOUND      = Set to true, if all components of MYSQL have been found.
#  MYSQL_INCLUDES   = Include path for the header files of MYSQL
#  MYSQL_LIBRARIES  = Link these to use MYSQL
#  MYSQL_LFLAGS     = Linker flags (optional)

if (NOT MYSQL_FOUND)
    
  ##_____________________________________________________________________________
  ## Check for the header files
  
  find_path (MYSQL_INCLUDES mysql/mysql.h
    PATHS /sw /usr /usr/local /opt/local ${CMAKE_INSTALL_PREFIX}
    PATH_SUFFIXES include
    )
  
  ##_____________________________________________________________________________
  ## Check for the library

  set (MYSQL_LIBRARIES "")

  find_library (MYSQL_MYSQLCLIENT_LIBRARY mysqlclient
    PATHS /sw /usr /usr/local /opt/local ${CMAKE_INSTALL_PREFIX}
    PATH_SUFFIXES lib lib/mysql
    )
  if (MYSQL_MYSQLCLIENT_LIBRARY)
    list (APPEND MYSQL_LIBRARIES ${MYSQL_MYSQLCLIENT_LIBRARY})
  endif (MYSQL_MYSQLCLIENT_LIBRARY)
  
  ##_____________________________________________________________________________
  ## Check for the executable
  
  find_program (MYSQL_MYSQL_EXECUTABLE mysql
    PATHS /sw /usr /usr/local /opt/local ${CMAKE_INSTALL_PREFIX}
    PATH_SUFFIXES bin share share/mysql share/mysql/bin
    )
  
  find_program (MYSQL_CONFIG_EXECUTABLE mysql_config
    PATHS /sw /usr /usr/local /opt/local ${CMAKE_INSTALL_PREFIX}
    PATH_SUFFIXES bin share share/mysql share/mysql/bin
    )
  
  ##_____________________________________________________________________________
  ## Test MqSQL installation for:
  
  if (MYSQL_INCLUDES AND MYSQL_LIBRARIES)
    ## Locate test program
    find_file (HAVE_TestMySQL TestMySQL.cc
      PATHS ${PROJECT_SOURCE_DIR}
      PATH_SUFFIXES cmake Modules
      )
    ## Build and run test program
    if (HAVE_TestMySQL)
      try_run(MYSQL_RUN_RESULT MYSQL_COMPILE_RESULT
	${PROJECT_BINARY_DIR}
	${HAVE_TestMySQL}
	CMAKE_FLAGS -DLINK_LIBRARIES:STRING=${MYSQL_LIBRARIES}
	COMPILE_DEFINITIONS -I${MYSQL_INCLUDES}
	RUN_OUTPUT_VARIABLE MYSQL_RUN_OUTPUT
	)
      ## Feedback from test
      message (STATUS "MYSQL_COMPILE_RESULT = ${MYSQL_COMPILE_RESULT}")
      message (STATUS "MYSQL_RUN_RESULT     = ${MYSQL_RUN_RESULT}")
      message (STATUS "MYSQL_RUN_OUTPUT     = ${MYSQL_RUN_OUTPUT}")
    else (HAVE_TestMySQL)
      message (STATUS "[MySQL] Unable to find test program TestMySQL.cc!")
    endif (HAVE_TestMySQL)
  endif (MYSQL_INCLUDES AND MYSQL_LIBRARIES)
  
  ##_____________________________________________________________________________
  ## Actions taken when all components have been found
  
  if (MYSQL_INCLUDES AND MYSQL_LIBRARIES)
    set (MYSQL_FOUND TRUE)
  else (MYSQL_INCLUDES AND MYSQL_LIBRARIES)
    set (MYSQL_FOUND FALSE)
    if (NOT MYSQL_FIND_QUIETLY)
      if (NOT MYSQL_INCLUDES)
	message (STATUS "Unable to find MYSQL header files!")
      endif (NOT MYSQL_INCLUDES)
      if (NOT MYSQL_LIBRARIES)
	message (STATUS "Unable to find MYSQL library files!")
      endif (NOT MYSQL_LIBRARIES)
    endif (NOT MYSQL_FIND_QUIETLY)
  endif (MYSQL_INCLUDES AND MYSQL_LIBRARIES)
  
  if (MYSQL_FOUND)
    if (NOT MYSQL_FIND_QUIETLY)
      message (STATUS "Found components for MYSQL")
      message (STATUS "MYSQL_INCLUDES  = ${MYSQL_INCLUDES}")
      message (STATUS "MYSQL_LIBRARIES = ${MYSQL_LIBRARIES}")
    endif (NOT MYSQL_FIND_QUIETLY)
  else (MYSQL_FOUND)
    if (MYSQL_FIND_REQUIRED)
      message (FATAL_ERROR "Could not find MYSQL!")
    endif (MYSQL_FIND_REQUIRED)
  endif (MYSQL_FOUND)
  
  ##_____________________________________________________________________________
  ## Mark advanced variables
  
  mark_as_advanced (
    MYSQL_INCLUDES
    MYSQL_LIBRARIES
    MYSQL_MYSQLCLIENT_LIBRARY
    )
  
endif (NOT MYSQL_FOUND)
