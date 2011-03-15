
## ==============================================================================
##
##  Check required packages
##
## ==============================================================================

## Effect of dependencies on build options; if we are unable to locate all the
## required components to generate the Python bindings to the DAL, we have to 
## disable this option - otherwise we won't be able to properly build the test
## programs and applications.

if (NOT BOOST_FOUND)
  set (DAL_PYTHON_BINDINGS FALSE)
  message (STATUS "[DAL] Unable to generate Python bindings; missing Boost++ headers!")
endif (NOT BOOST_FOUND)

if (NOT PYTHON_INCLUDES)
  set (DAL_PYTHON_BINDINGS FALSE)
  message (STATUS "[DAL] Unable to generate Python bindings; missing Python headers!")
endif (NOT PYTHON_INCLUDES)

if (NOT NUMUTIL_INCLUDES) 
  set (DAL_PYTHON_BINDINGS FALSE)
  message (STATUS "[DAL] Unable to generate Python bindings; missing num_util headers!")
endif (NOT NUMUTIL_INCLUDES)

## ==============================================================================
##
##  
##
## ==============================================================================

if (DAL_PYTHON_BINDINGS)
  
  file (WRITE ${PROJECT_BINARY_DIR}/CMakeTmp/CMakeLists.txt
    "project (TestForPythonBindings)
     cmake_minimum_required(VERSION 2.6)
     include_directories (${PYTHON_INCLUDES})
     include_directories (${BOOST_INCLUDES})
     include_directories (${NUMUTIL_INCLUDES} ${NUMUTIL_INCLUDES}/num_util)
     ADD_EXECUTABLE(TestForPythonBindings ${DAL_SOURCE_DIR}/cmake/TestPythonBindings.cc)
     TARGET_LINK_LIBRARIES(TestForPythonBindings ${PYTHON_LIBRARIES} ${NUMUTIL_LIBRARIES})"
    )
  
  try_compile (DAL_PYTHON_BINDINGS
    ${PROJECT_BINARY_DIR}/CMakeTmp
    ${PROJECT_BINARY_DIR}/CMakeTmp
    TestForPythonBindings
    )
  
  ## Enable include directories of test compile succeeded
  
  if (DAL_PYTHON_BINDINGS AND PYTHON_INCLUDES)
    include_directories (${PYTHON_INCLUDES})
  else (DAL_PYTHON_BINDINGS AND PYTHON_INCLUDES)
    message (STATUS "[DAL] Test compilation of Python bindings failed; disabling")
    set (DAL_PYTHON_BINDINGS FALSE)
  endif (DAL_PYTHON_BINDINGS AND PYTHON_INCLUDES)
  
  ## We need to be a bit careful with the Boost and Python libraries; appending
  ## the Boost libraries only makes sense, if there is already a list of Python
  ## libraries. Furthermore we need to make sure not to have libboost_python in
  ## in the list, because this requires for the presence of the Python library.
  
  if (BOOST_LIBRARIES)
    if (PYTHON_LIBRARIES)
      list (APPEND PYTHON_LIBRARIES ${BOOST_LIBRARIES})
    else (PYTHON_LIBRARIES)
      ## Clean up the list of Boost libraries
      foreach (lib ${BOOST_LIBRARIES})
	string (REGEX MATCH ^.*python.* have_libboost_python ${lib})
	if (have_libboost_python)
	  list (REMOVE_ITEM BOOST_LIBRARIES ${have_libboost_python})
	endif (have_libboost_python)
      endforeach (lib)
      ## Add the libraries
      set (PYTHON_LIBRARIES ${BOOST_LIBRARIES})
    endif (PYTHON_LIBRARIES)
  endif (BOOST_LIBRARIES)
  
endif (DAL_PYTHON_BINDINGS)

