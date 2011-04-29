
## ==============================================================================
##
## Build the library for the Python bindings, "pydal"
##
## ==============================================================================

##______________________________________________
## Source files with the wrapping code

file (GLOB pydal_sources *.cc)

##______________________________________________
## Compiler instructions

remove_definitions (-dynamiclib -install_name)

if (NUMPY_INCLUDES)
  include_directories (${NUMPY_INCLUDES})
endif (NUMPY_INCLUDES)

add_library(pydal MODULE
  ${pydal_sources}
  )

##______________________________________________
## linker instructions

target_link_libraries (pydal
  dal
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
