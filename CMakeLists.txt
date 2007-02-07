
## Project name

project (DAL)

## subdirectories to process

subdirs (
  implement
  test
)

## search for required external components

include (${DAL_SOURCE_DIR}/../../devel_common/cmake/FindHDF5.cmake)
include (${DAL_SOURCE_DIR}/../../devel_common/cmake/FindCFITSIO.cmake)
