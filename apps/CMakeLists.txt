## ------------------------------------------------------------------------------
## $Id::                                                                        $
## ------------------------------------------------------------------------------

set(CMAKE_CXX_FLAGS "-W -Wall")

## ==============================================================================
##
##  Build application programs
##
## ==============================================================================

if (LOFAR_FOUND)
  add_subdirectory (bf2h5)
endif (LOFAR_FOUND)

##____________________________________________________________________
## Applications with no further external dependencies

## source files
set (tests read_tbb)
## linker instructions
set (apps_link_libraries dal)

##____________________________________________________________________
## Applications which require casacore

if (CASA_FOUND OR CASACORE_FOUND)
  ## source files
  list (APPEND tests msread)
  list (APPEND tests ms2h5)
  list (APPEND tests lopes2h5)
  ## linker instructions
  list (APPEND apps_link_libraries
    ${dal_link_libraries}
    )
  ## remove duplicates from the list of link libraries
  list (REMOVE_DUPLICATES apps_link_libraries)
else (CASA_FOUND OR CASACORE_FOUND)
  message (STATUS "[DAL] Unable to build applications: msread, ms2h5")
endif (CASA_FOUND OR CASACORE_FOUND)

##____________________________________________________________________
## Applications which require Boost

if (HAVE_BOOST)
  if (BOOST_THREAD_LIBRARY)
    ## source files
    list (APPEND tests bf2h5_old)
    list (APPEND tests tbb2h5)
    list (APPEND tests TBBraw2h5)
    ## linker instructions
    list (APPEND apps_link_libraries
      ${BOOST_LIBRARIES}
      )
  else (BOOST_THREAD_LIBRARY)
    message (STATUS "[DAL] Missing Boost++ thread library!")
  endif (BOOST_THREAD_LIBRARY)
else (HAVE_BOOST)
  message (STATUS "[DAL] Missing Boost++ -- unable to build applications!")
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

  ## Test bf2h5_old with various command line parameters

  add_test (bf2h5_old_test1 bf2h5_old)
  add_test (bf2h5_old_test2 bf2h5_old -H)
  add_test (bf2h5_old_test3 bf2h5_old --help)
  add_test (bf2h5_old_test4 bf2h5_old --intensity)
  
  if (dataset_bf_raw)
    add_test (bf2h5_old_test6 bf2h5_old -I ${dataset_bf_raw} -O testdata_bf.h5)
    set_tests_properties (
      bf2h5_old_test6
      PROPERTIES
      TIMEOUT 600
      )
  else (dataset_bf_raw)
    message (STATUS "[DAL] Skipping bf2h5_old_test6 - no input file!")
  endif (dataset_bf_raw)
  
endif (DAL_ENABLE_TESTING)

## ==============================================================================
##
## Installation
##
## ==============================================================================

install (TARGETS ${tests}
  RUNTIME DESTINATION ${DAL_INSTALL_BINDIR}
  LIBRARY DESTINATION ${DAL_INSTALL_LIBDIR}
  )

## scripts

file (GLOB python_scripts *.py)

install (FILES ${python_scripts}
  DESTINATION ${DAL_INSTALL_BINDIR}
  PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE OWNER_READ GROUP_READ WORLD_READ
)
