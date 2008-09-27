## ------------------------------------------------------------------------------
## $Id::                                                                        $
## ------------------------------------------------------------------------------

set(CMAKE_CXX_FLAGS "-W -Wall")
message(STATUS "cflags: ${CMAKE_CXX_FLAGS}")

#add_definitions(
#  -DMPICH_IGNORE_CXX_SEEK
#  -D_BSD_SOURCE
#  -D_POSIX_SOURCE
#  -D_FILE_OFFSET_BITS=64
#  -D_LARGEFILE64_SOURCE
#  -D_LARGEFILE_SOURCE
#  )

## ==============================================================================
##
##  Build application programs
##
## ==============================================================================

## ------------------------------------------------------------------------------
## Set up the linker instructions for the applications

set (apps_link_libraries dal)

if (HAVE_CASA OR HAVE_CASACORE)
  list (APPEND apps_link_libraries ${dal_link_libraries})
endif (HAVE_CASA OR HAVE_CASACORE)

if (HAVE_BOOST)
  list (APPEND apps_link_libraries ${BOOST_LIBRARIES})
endif (HAVE_BOOST)

## ------------------------------------------------------------------------------
## Build application programs

set (tests
  msread
  ms2h5
  tbb2h5
  bf2h5
  read_tbb
  )

foreach (src ${tests})
  ## executable and its source
  add_executable(${src} ${src}.cpp)
  ## libraries against which to link
  target_link_libraries(${src} ${apps_link_libraries})
endforeach (src)

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

  if (dataset_tbb_raw)
    add_test (tbb2h5_test9 tbb2h5 --infile ${dataset_tbb_raw} --outfile testdata.h5)
  endif (dataset_tbb_raw)

  ## Test bf2h5 with various command line parameters

  add_test (bf2h5_test1 bf2h5)
  add_test (bf2h5_test2 bf2h5 -H)
  add_test (bf2h5_test3 bf2h5 --help)
  add_test (bf2h5_test4 bf2h5 --downsample)
  add_test (bf2h5_test5 bf2h5 --intensity)

endif (DAL_ENABLE_TESTING)

## ------------------------------------------------------------------------------
## Programs which require additional external libraries (and thereby should be
## considered optional)

## -------------------------------------------------------------------
## lopes2h5

if (HAVE_CASA OR HAVE_CASACORE)
  ## additional compiler flags
  add_definitions (
    -DWITH_CASA
    )
  foreach (src lopes2h5)
    ## compiler instructions
    add_executable (${src} ${src}.cpp)
    ## linker instructions
    target_link_libraries (${src}  dal ${dal_link_libraries})
    ## installation
    list (APPEND tests ${src})
  endforeach (src)
else (HAVE_CASA OR HAVE_CASACORE)
  message (STATUS "Skipping test programs that require casacore!")
endif (HAVE_CASA OR HAVE_CASACORE)

## ------------------------------------------------------------------------------
## Installation

## test programs

install (TARGETS ${tests}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  )
