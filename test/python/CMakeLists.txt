
file (GLOB have_datacube datacube.py)
file (GLOB have_uv_coverage uv_coverage.py)
file (GLOB have_baseline_intensity baseline_intensity.py)

## === Run tests working with data ==============================================

if (dataset_ms)

  ## show the coverage in the (u,v) plane
  add_test (datacube ${PYTHON_EXECUTABLE} ${have_datacube} ${dataset_ms} ${dataset_ms}.out 1 1 0)
  ## show the coverage in the (u,v) plane
  add_test (uv_coverage ${PYTHON_EXECUTABLE} ${have_uv_coverage} ${dataset_ms})
  ## auto-correlated intensity in the first frequency band
  add_test (baseline_intensity1 ${PYTHON_EXECUTABLE} ${have_baseline_intensity} ${dataset_ms} 1 1 0)
  ## auto-correlated intensity in the first frequency band, channel 0
  add_test (baseline_intensity2 ${PYTHON_EXECUTABLE} ${have_baseline_intensity} ${dataset_ms} 1 1 0 0)

endif (dataset_ms)

## === Installation instructions ================================================

file (GLOB python_scripts *.py)

install (FILES ${python_scripts}
  DESTINATION ${DAL_INSTALL_BINDIR}
  PERMISSIONS OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE OWNER_READ GROUP_READ WORLD_READ
)
