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

  if (NOT PYTHON_FOUND)
    set (PYTHON_FIND_QUIETLY ${NUMPY_FIND_QUIETLY})
    include (FindPython_DAL)
  endif (NOT PYTHON_FOUND)

  set (NUMPY_FOUND FALSE)
  set (NUMPY_INCLUDES "")

  if (PYTHON_EXECUTABLE)
    execute_process(
      COMMAND ${PYTHON_EXECUTABLE} -c import\ numpy\;\ print\ numpy.get_include\(\)
      RESULT_VARIABLE NUMPY_FIND_ERROR
      OUTPUT_VARIABLE NUMPY_INCLUDES
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
  endif (PYTHON_EXECUTABLE)

  if (NOT NUMPY_FIND_ERROR)
    set (NUMPY_FOUND TRUE)
    if (NOT NUMPY_FIND_QUIETLY)
      message (STATUS "Found components for NumPy")
      message (STATUS "NUMPY_INCLUDES  = ${NUMPY_INCLUDES}")
    endif (NOT NUMPY_FIND_QUIETLY)
  else (NOT NUMPY_FIND_ERROR)
    set (NUMPY_FOUND FALSE)
    if (NOT NUMPY_FIND_QUIETLY)
      message (FATAL_ERROR "Attempt to import NumPy failed!")
    endif (NOT NUMPY_FIND_QUIETLY)
  endif (NOT NUMPY_FIND_ERROR)

  ##_____________________________________________________________________________
  ## Mark advanced variables

  mark_as_advanced (
    NUMPY_INCLUDES
    )

endif (NOT NUMPY_FOUND)
