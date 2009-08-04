## ------------------------------------------------------------------------------
## $Id::                                                                        $
## ------------------------------------------------------------------------------

set(CMAKE_CXX_FLAGS "-W -Wall")
message(STATUS "CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")

## ==============================================================================
##
##  Build application programs
##
## ==============================================================================

##____________________________________________________________________
## Applications with no further external dependencies

## source files
set (tests read_tbb)
## linker instructions
set (apps_link_libraries dal)

##____________________________________________________________________
## Applications which require casacore

if (HAVE_CASA OR HAVE_CASACORE)
  ## source files
  list (APPEND tests msread)
  list (APPEND tests ms2h5)
  list (APPEND tests lopes2h5)
  ## linker instructions
  list (APPEND apps_link_libraries ${dal_link_libraries})
else (HAVE_CASA OR HAVE_CASACORE)
  message (STATUS "[DAL] Unable to build applications: msread, ms2h5")
endif (HAVE_CASA OR HAVE_CASACORE)

##____________________________________________________________________
## Applications which require Boost

if (HAVE_BOOST)
  ## source files
  list (APPEND tests bf2h5)
  list (APPEND tests tbb2h5)
  list (APPEND tests TBBraw2h5)
  ## linker instructions
  # boost_python won't work on some platforms unless libpython is also linked.
  list (APPEND apps_link_libraries ${BOOST_LIBRARIES} ${CFITSIO_LIBRARIES} ${PYTHON_LIBRARIES})
  # Is this needed for the applications? It might be nicer to strip it out...
#  foreach (lib ${apps_link_libraries})
#    string (REGEX MATCH ^.*python.* have_libboost_python ${lib})
#    if (have_libboost_python)
#      list (REMOVE_ITEM apps_link_libraries ${have_libboost_python})
#    endif (have_libboost_python)
#  endforeach (lib)
else (HAVE_BOOST)
  message (STATUS "[DAL] Unable to build applications: bf2h5, tbb2h5")
endif (HAVE_BOOST)

##____________________________________________________________________
## Build the executables

foreach (src ${tests})
  ## compiler instructions
  add_executable(${src} ${src}.cpp)
  ## linker instructions
  target_link_libraries(${src} ${apps_link_libraries})
endforeach (src)

## ==============================================================================
##
##  Run the test programs
##
## ==============================================================================

if (DAL_ENABLE_TESTING)

  ## Test tbb2h5 with various command line parameters

  add_test (tbb2h5_test1 tbb2h5 -H)
  add_test (tbb2h5_test2 tbb2h5 --help)
  add_test (tbb2h5_test3 tbb2h5 -O testdata.h5)
  add_test (tbb2h5_test4 tbb2h5 --outfile testdata.h5)
  add_test (tbb2h5_test5 tbb2h5 -M 0)
  add_test (tbb2h5_test6 tbb2h5 --mode 0)
  add_test (tbb2h5_test7 tbb2h5 -P 20)
  add_test (tbb2h5_test8 tbb2h5 --port 20)
  add_test (tbb2h5_test9 tbb2h5 --port 20 --timeoutRead 0.2)

  if (dataset_tbb_raw)
    add_test (tbb2h5_test10 tbb2h5 --infile ${dataset_tbb_raw} --outfile testdata.h5)
    add_test (tbb2h5_test11 tbb2h5 --infile ${dataset_tbb_raw} --outfile testdata.h5)
    add_test (tbb2h5_test12 tbb2h5 --infile ${dataset_tbb_raw} --outfile testdata.h5)
  endif (dataset_tbb_raw)

  ## Test bf2h5 with various command line parameters

  add_test (bf2h5_test1 bf2h5)
  add_test (bf2h5_test2 bf2h5 -H)
  add_test (bf2h5_test3 bf2h5 --help)
  add_test (bf2h5_test4 bf2h5 --downsample)
  add_test (bf2h5_test5 bf2h5 --intensity)
  
  if (dataset_bf_raw)
    add_test (bf2h5_test6 bf2h5 -I ${dataset_bf_raw} -O testdata_bf.h5)
    add_test (bf2h5_test7 bf2h5 -I ${dataset_bf_raw} -O testdata_bf.h5 --intensity)
  else (dataset_bf_raw)
    message (STATUS "[DAL] Skipping bf2h5 conversion tests - no input file!")
  endif (dataset_bf_raw)
  
endif (DAL_ENABLE_TESTING)

## ==============================================================================
##
## Installation
##
## ==============================================================================

install (TARGETS ${tests}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  )
