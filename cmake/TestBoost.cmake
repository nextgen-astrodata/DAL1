
##_____________________________________________________________________________
## Test Boost library for library version <major.minor.release>

## Locate test program
find_file (TEST_BOOST_CC TestBoost.cc
  PATHS ${PROJECT_SOURCE_DIR}
  PATH_SUFFIXES cmake Modules
  )

if (BOOST_INCLUDES AND BOOST_LIBRARIES AND TEST_BOOST_CC)
  try_run(BOOST_VERSION_RUN_RESULT BOOST_VERSION_COMPILE_RESULT
    ${PROJECT_BINARY_DIR}
    ${TEST_BOOST_CC}
    COMPILE_DEFINITIONS -I${BOOST_INCLUDES}
    RUN_OUTPUT_VARIABLE BOOST_VERSION_OUTPUT
    )
endif (BOOST_INCLUDES AND BOOST_LIBRARIES AND TEST_BOOST_CC)

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
    message (STATUS "[Boost] Failed to run TestBoost!")
  endif (BOOST_VERSION_RUN_RESULT AND BOOST_VERSION_OUTPUT)
else (BOOST_VERSION_COMPILE_RESULT)
  message (STATUS "[Boost] Failed to compile TestBoost!")
endif (BOOST_VERSION_COMPILE_RESULT)

## Assemble full version of library
set (BOOST_VERSION "${BOOST_VERSION_MAJOR}.${BOOST_VERSION_MINOR}.${BOOST_VERSION_PATCH}")

##_____________________________________________________________________________
## Test Boost library for library version <major.minor.release>


## Test support of thread library

if (BOOST_INCLUDES AND Boost_THREAD_LIBRARY AND TEST_BOOST_CC)
  try_run(BOOST_THREAD_RUN_RESULT BOOST_THREAD_COMPILE_RESULT
    ${PROJECT_BINARY_DIR}
    ${TEST_BOOST_CC}
    CMAKE_FLAGS -DLINK_LIBRARIES:STRING=${Boost_THREAD_LIBRARY}\;${PTHREAD_LIBRARY}
    COMPILE_DEFINITIONS -I${BOOST_INCLUDES} -DTEST_BOOST_THREAD
    COMPILE_OUTPUT_VARIABLE BOOST_THREAD_COMPILE_OUTPUT
    RUN_OUTPUT_VARIABLE BOOST_THREAD_RUN_OUTPUT
    )
endif (BOOST_INCLUDES AND Boost_THREAD_LIBRARY AND TEST_BOOST_CC)

if (BOOST_THREAD_COMPILE_RESULT)
  if (BOOST_THREAD_RUN_RESULT)
    message (STATUS "[Boost] Thread support enabled.")
    set (BOOST_WITH_THREAD YES)
  else (BOOST_THREAD_RUN_RESULT)
    set (BOOST_WITH_THREAD NO)
    message (STATUS "[Boost] Failed to run TestBoost with thread support!")
  endif (BOOST_THREAD_RUN_RESULT)
else (BOOST_THREAD_COMPILE_RESULT)
  set (BOOST_WITH_THREAD NO)
  message (STATUS "[Boost] Failed to compile TestBoost with thread support!")
  if (DAL_VERBOSE_CONFIGURE)
    message (${BOOST_THREAD_COMPILE_OUTPUT})
  endif (DAL_VERBOSE_CONFIGURE)
endif (BOOST_THREAD_COMPILE_RESULT)

