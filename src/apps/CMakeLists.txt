
set(CMAKE_CXX_FLAGS "-W -Wall")

## ==============================================================================
##
##  Build application programs
##
## ==============================================================================

#if (LOFAR_FOUND)
#  add_subdirectory (bf2h5)
#endif (LOFAR_FOUND)

##____________________________________________________________________
## Configuration overview

if (Boost_PROGRAM_OPTIONS_LIBRARY)
  ## Compiler instructions
  add_executable (dal-config dal-config.cc)
  ## Linker instructions
  target_link_libraries (dal-config ${Boost_PROGRAM_OPTIONS_LIBRARY})
  ## Installation
  install (TARGETS dal-config
    RUNTIME DESTINATION ${DAL_INSTALL_BINDIR}
    LIBRARY DESTINATION ${DAL_INSTALL_LIBDIR}
    )
  ## Testing
  add_test (dal-config                dal-config)
  add_test (dal-config-version        dal-config --version)
  add_test (dal-config-all            dal-config --all)
  add_test (dal-config-versionMajor   dal-config --version-major)
  add_test (dal-config-versionMinor   dal-config --version-minor)
  add_test (dal-config-versionPath    dal-config --version-patch)
  add_test (dal-config-versionCMake   dal-config --version-cmake)
  add_test (dal-config-versionSystem  dal-config --version-system)
endif (Boost_PROGRAM_OPTIONS_LIBRARY)

##____________________________________________________________________
## Applications with no further external dependencies

## source files
set (tests read_tbb)
## linker instructions
set (apps_link_libraries dal)

add_executable (udp-copy udp-copy.c)

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
##                                                              tbb2h5
##                                                           TBBraw2h5

if (Boost_PROGRAM_OPTIONS_LIBRARY AND Boost_THREAD_LIBRARY)
  ## compiler instructions
  add_executable (tbb2h5    tbb2h5.cpp   )
  add_executable (TBBraw2h5 TBBraw2h5.cpp)
  ## linker instructions
  target_link_libraries (tbb2h5
    dal
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    )
  target_link_libraries (TBBraw2h5
    dal
    ${Boost_PROGRAM_OPTIONS_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    )
  ## Installation instructions
  install (TARGETS tbb2h5 TBBraw2h5
    RUNTIME DESTINATION ${DAL_INSTALL_BINDIR}
    LIBRARY DESTINATION ${DAL_INSTALL_LIBDIR}
    )
else (Boost_PROGRAM_OPTIONS_LIBRARY AND Boost_THREAD_LIBRARY)
    message (STATUS "[DAL] Unable to build tbb2h5 - missing Boost++ libraries!")
    message (STATUS "[DAL] Unable to build TBBraw2h5 - missing Boost++ libraries!")
endif (Boost_PROGRAM_OPTIONS_LIBRARY AND Boost_THREAD_LIBRARY)

##____________________________________________________________________
##                                                               tbbmd

if (Boost_PROGRAM_OPTIONS_LIBRARY)
  if (CASA_LIBRARIES)
    ## compiler instructions
    add_executable (tbbmd tbbmd.cpp)
    ## linker instructions
    target_link_libraries (tbbmd
      dal
      ${Boost_PROGRAM_OPTIONS_LIBRARY}
      ${CASA_LIBRARIES}
      )
    ## Installation instructions
    install (TARGETS tbbmd
      RUNTIME DESTINATION ${DAL_INSTALL_BINDIR}
      LIBRARY DESTINATION ${DAL_INSTALL_LIBDIR}
      )
  else (CASA_LIBRARIES)
    message (STATUS "[DAL] Unable to build tbbmd - missing casacore library!")
  endif (CASA_LIBRARIES)
else (Boost_PROGRAM_OPTIONS_LIBRARY)
  message (STATUS "[DAL] Unable to build tbbmd - missing Boost++ program_options library!")
endif (Boost_PROGRAM_OPTIONS_LIBRARY)

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
