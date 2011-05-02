
## === Library header files =====================================================

include_directories (
  ${DAL_BINARY_DIR}
  ${DAL_SOURCE_DIR}/src
  ${DAL_SOURCE_DIR}/src/dal
  ${DAL_SOURCE_DIR}/src/apps
  )

## === Directories to build libraries ===========================================

add_subdirectory (dal)
add_subdirectory (apps)

if (DAL_PYTHON_BINDINGS)
  add_subdirectory (pydal)
endif (DAL_PYTHON_BINDINGS)

## === Directories with test programs ===========================================

if (DAL_ENABLE_TESTING)
  ## include paths
  include_directories (
    ${CMAKE_CURRENT_SOURCE_DIR}/test
    ${CMAKE_CURRENT_SOURCE_DIR}/test/hdf5
    )
  ## add source directory
  add_subdirectory (test)
endif (DAL_ENABLE_TESTING)
