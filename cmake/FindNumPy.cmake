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

if (NOT NUMPY_FOUND)

  ## Initialize variables

  unset (NUMPY_FOUND)
  unset (NUMPY_INCLUDES)

  ##_____________________________________________________________________________
  ## Check for installation of Python

  if (NOT PYTHON_FOUND)
    set (PYTHON_FIND_QUIETLY ${NUMPY_FIND_QUIETLY})
    include (FindPython_DAL)
  endif (NOT PYTHON_FOUND)

  ##_____________________________________________________________________________
  ## Search for header files

  if (PYTHON_EXECUTABLE)
    ## Use Python to determine the include directory
    execute_process (
      COMMAND ${PYTHON_EXECUTABLE} -c import\ numpy\;\ print\ numpy.get_include\(\)\;
      ERROR_VARIABLE NUMPY_FIND_ERROR
      RESULT_VARIABLE NUMPY_FIND_RESULT
      OUTPUT_VARIABLE NUMPY_FIND_OUTPUT
      OUTPUT_STRIP_TRAILING_WHITESPACE
      )
    ## process the output from the execution of the command
    if (NOT NUMPY_FIND_RESULT)
      set (NUMPY_INCLUDES ${NUMPY_FIND_OUTPUT})
    endif (NOT NUMPY_FIND_RESULT)
  endif (PYTHON_EXECUTABLE)
  
  ##_____________________________________________________________________________
  ## Actions taken after completing the search

  if (NUMPY_INCLUDES)
    set (NUMPY_FOUND TRUE)
    if (NOT NUMPY_FIND_QUIETLY)
      message (STATUS "[FindNumPy] Found components for NumPy")
      message (STATUS "NUMPY_INCLUDES  = ${NUMPY_INCLUDES}")
    endif (NOT NUMPY_FIND_QUIETLY)
  else (NUMPY_INCLUDES)
    set (NUMPY_FOUND FALSE)
    if (NOT NUMPY_FIND_QUIETLY)
      message (FATAL_ERROR "[FindNumPy] Attempt to import NumPy failed!")
    endif (NOT NUMPY_FIND_QUIETLY)
  endif (NUMPY_INCLUDES)

  ##_____________________________________________________________________________
  ## Mark advanced variables

  mark_as_advanced (
    NUMPY_INCLUDES
    )

endif (NOT NUMPY_FOUND)
