
## List of test scripts

file (GLOB pydal_tests *.py)

## 

foreach (_pydal_test ${pydal_tests})
  ## get filename components
  get_filename_component (_test_name      ${_pydal_test} NAME_WE)
  get_filename_component (_test_extension ${_pydal_test} EXT)
  ## Add test
  add_test (pydal_${_test_name} ${PYTHON_EXECUTABLE} ${_pydal_test})
endforeach (_pydal_test)

## Installation

install (FILES ${pydal_tests}
  DESTINATION bin
  PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE OWNER_READ GROUP_READ WORLD_READ
)
