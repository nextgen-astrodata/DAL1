
## Project name

project (DAL)

## subdirectories to process

subdirs (
  implement
  test
)

## ------------------------------------------------------------------------------
## External dependencies

if (NOT HAVE_HDF5)
  include (${DAL_SOURCE_DIR}/../../devel_common/cmake/FindHDF5.cmake)
endif (NOT HAVE_HDF5)

if (NOT HAVE_CFITSIO)
  include (${DAL_SOURCE_DIR}/../../devel_common/cmake/FindCFITSIO.cmake)
endif (NOT HAVE_CFITSIO)

if (NOT HAVE_BLITZ)
  include (${DAL_SOURCE_DIR}/../../devel_common/cmake/FindBlitz.cmake)
endif (NOT HAVE_BLITZ)
