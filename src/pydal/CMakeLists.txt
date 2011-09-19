
## ==============================================================================
##
## Build the library for the Python bindings, "pydal"
##
## ==============================================================================

##______________________________________________
## Source files with the wrapping code

set (pydal_sources
  num_util.cc
  pydal.cc
  core/pydal_HDF5Dataset.cc
  core/pydal_HDF5Hyperslab.cc
  coordinates/pydal_Angle.cc
  coordinates/pydal_Coordinate.cc
  coordinates/pydal_RaDec.cc
  coordinates/pydal_Stokes.cc
  pydal_bf.cc
  pydal_core.cc
  pydal_core_dalArray.cc
  pydal_core_dalColumn.cc
  pydal_core_dalDataset.cc
  pydal_core_dalFileType.cc
  pydal_core_dalGroup.cc
  pydal_core_IO_Mode.cc
  pydal_core_dalTable.cc
  pydal_data_common.cc
  pydal_data_hl.cc
  data_common/pydal_Filename.cc
  data_common/pydal_Timestamp.cc
  data_hl/pydal_BeamFormed.cc
  data_hl/pydal_BeamGroup.cc
  data_hl/pydal_TBB_Timeseries.cc
  data_hl/pydal_TBB_DipoleDataset.cc
  )

##______________________________________________
## Compiler instructions

remove_definitions (-dynamiclib -install_name)

include_directories (${CMAKE_CURRENT_SOURCE_DIR})

if (NUMPY_INCLUDES)
  include_directories (${NUMPY_INCLUDES})
endif (NUMPY_INCLUDES)

if (PYTHON_INCLUDES)
  include_directories (${PYTHON_INCLUDES})
endif (PYTHON_INCLUDES)

add_library(pydal MODULE
  ${pydal_sources}
  )

##______________________________________________
## linker instructions

target_link_libraries (pydal
  dal
  ${Boost_PYTHON_LIBRARY}
  ${PYTHON_LIBRARIES}
  )

##______________________________________________
## Additional target properties

if (APPLE)
  set_target_properties (pydal
    PROPERTIES
    PREFIX ""
    SUFFIX .so
    COMPILE_FLAGS "-DPYTHON -fpermissive"
    LINK_FLAGS "-fPIC -flat_namespace"
    )
else (APPLE)
  set_target_properties (pydal
    PROPERTIES
    PREFIX ""
    SUFFIX .so
    COMPILE_FLAGS -DPYTHON
    LINK_FLAGS "-fPIC -shared"
    )
endif (APPLE)

##______________________________________________
## Installation instructions

install (
  TARGETS pydal
  LIBRARY DESTINATION ${DAL_INSTALL_LIBDIR}
  )

##______________________________________________
## Test programs for this module

add_subdirectory (test)
