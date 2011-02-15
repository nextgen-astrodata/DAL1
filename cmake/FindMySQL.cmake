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

  if (NOT MYSQL_ROOT_DIR)
    set (MYSQL_ROOT_DIR ${CMAKE_INSTALL_PREFIX})
  endif (NOT MYSQL_ROOT_DIR)
  
  set (MYSQL_VERSION  0     )
  set (MYSQL_PORT     0     )
  set (MYSQL_FOUND    FALSE )
    
  ##_____________________________________________________________________________
  ## Check for the header files
  
  find_path (MYSQL_INCLUDES mysql.h
    HINTS ${MYSQL_ROOT_DIR}
    PATHS /sw /usr /usr/local /opt/local
    PATH_SUFFIXES include include/mysql
    )
  
  ##_____________________________________________________________________________
  ## Check for the library

  set (MYSQL_LIBRARIES "")

  ## libmysqlclient

  find_library (MYSQL_MYSQLCLIENT_LIBRARY libmysqlclient.a mysqlclient
    HINTS ${MYSQL_ROOT_DIR}
    PATHS /sw /usr /usr/local /opt/local
    PATH_SUFFIXES lib lib/mysql
    )
  if (MYSQL_MYSQLCLIENT_LIBRARY)
    list (APPEND MYSQL_LIBRARIES ${MYSQL_MYSQLCLIENT_LIBRARY})
  endif (MYSQL_MYSQLCLIENT_LIBRARY)

  ## libmysqlservices

  find_library (MYSQL_MYSQLSERVICES_LIBRARY libmysqlservices.a mysqlservices
    HINTS ${MYSQL_ROOT_DIR}
    PATHS /sw /usr /usr/local /opt/local
    PATH_SUFFIXES lib lib/mysql
    )
  if (MYSQL_MYSQLSERVICES_LIBRARY)
    list (APPEND MYSQL_LIBRARIES ${MYSQL_MYSQLSERVICES_LIBRARY})
  endif (MYSQL_MYSQLSERVICES_LIBRARY)

  ## libmysql
  
  find_library (MYSQL_MYSQL_LIBRARY mysql libmysql.a
    HINTS ${MYSQL_ROOT_DIR}
    PATHS /sw /usr /usr/local /opt/local
    PATH_SUFFIXES lib lib/mysql
    )
  if (MYSQL_MYSQL_LIBRARY)
    list (APPEND MYSQL_LIBRARIES ${MYSQL_MYSQL_LIBRARY})
  endif (MYSQL_MYSQL_LIBRARY)
  
  ##_____________________________________________________________________________
  ## Check for the executable
  
  find_program (MYSQL_MYSQL_EXECUTABLE mysql
    HINTS ${MYSQL_ROOT_DIR}
    PATHS /sw /usr /usr/local /opt/local
    PATH_SUFFIXES bin share share/mysql share/mysql/bin
    )
  
  find_program (MYSQL_CONFIG_EXECUTABLE mysql_config
    HINTS ${MYSQL_ROOT_DIR}
    PATHS /sw /usr /usr/local /opt/local
    PATH_SUFFIXES bin share share/mysql share/mysql/bin
    )
  
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
      message (STATUS "MYSQL_VERSION   = ${MYSQL_VERSION}  ")
      message (STATUS "MYSQL_PORT      = ${MYSQL_PORT}     ")
      message (STATUS "MYSQL_INCLUDES  = ${MYSQL_INCLUDES} ")
      message (STATUS "MYSQL_LIBRARIES = ${MYSQL_LIBRARIES}")
    endif (NOT MYSQL_FIND_QUIETLY)
  else (MYSQL_FOUND)
    if (MYSQL_FIND_REQUIRED)
      message (FATAL_ERROR "Could not find MYSQL!")
    endif (MYSQL_FIND_REQUIRED)
  endif (MYSQL_FOUND)
  
  ##_____________________________________________________________________________
  ## Test MqSQL installation for:
  ## - version number
  ## - port number

  if (MYSQL_CONFIG_EXECUTABLE)

    ## Determine version number
    execute_process(
      COMMAND ${MYSQL_CONFIG_EXECUTABLE} --version
      RESULT_VARIABLE MYSQL_VERSION_RESULT_VARIABLE
      OUTPUT_VARIABLE MYSQL_VERSION_OUTPUT_VARIABLE
      OUTPUT_STRIP_TRAILING_WHITESPACE
      )

    if (MYSQL_VERSION_OUTPUT_VARIABLE)
      set (MYSQL_VERSION ${MYSQL_VERSION_OUTPUT_VARIABLE})
    else (MYSQL_VERSION_OUTPUT_VARIABLE)
      set (MYSQL_VERSION 0)
    endif (MYSQL_VERSION_OUTPUT_VARIABLE)
    
    ## Determine port number
    execute_process(
      COMMAND ${MYSQL_CONFIG_EXECUTABLE} --port
      RESULT_VARIABLE MYSQL_PORT_RESULT_VARIABLE
      OUTPUT_VARIABLE MYSQL_PORT_OUTPUT_VARIABLE
      OUTPUT_STRIP_TRAILING_WHITESPACE
      )

    if (MYSQL_PORT_OUTPUT_VARIABLE)
      set (MYSQL_PORT ${MYSQL_PORT_OUTPUT_VARIABLE})
    else (MYSQL_PORT_OUTPUT_VARIABLE)
      set (MYSQL_PORT 0)
    endif (MYSQL_PORT_OUTPUT_VARIABLE)

  endif (MYSQL_CONFIG_EXECUTABLE)
  
  if (MYSQL_FOUND)
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
	COMPILE_OUTPUT_VARIABLE MYSQL_COMPILE_OUTPUT
	RUN_OUTPUT_VARIABLE MYSQL_RUN_OUTPUT
	)
    else (HAVE_TestMySQL)
      message (STATUS "[MySQL] Unable to find test program TestMySQL.cc!")
    endif (HAVE_TestMySQL)
  endif (MYSQL_FOUND)
  
  ## Comile of test program successful?
  if (MYSQL_VERSION_COMPILE_RESULT)
    ## Run of test program successful?
    if (MYSQL_VERSION_RUN_RESULT)
    else (MYSQL_VERSION_RUN_RESULT)
      message (STATUS "[MySQL] Failed to run TestMySQL!")
      set (MYSQL_FOUND FALSE)
    endif (MYSQL_VERSION_RUN_RESULT)
  else (MYSQL_VERSION_COMPILE_RESULT)
    message (STATUS "[MySQL] Failed to compile TestMySQL!")
    set (MYSQL_FOUND FALSE)
  endif (MYSQL_VERSION_COMPILE_RESULT)
  
  ##_____________________________________________________________________________
  ## Mark advanced variables
  
  mark_as_advanced (
    MYSQL_INCLUDES
    MYSQL_LIBRARIES
    MYSQL_MYSQL_LIBRARY
    MYSQL_MYSQLCLIENT_LIBRARY
    )
  
endif (NOT MYSQL_FOUND)
