
file (GLOB pydal_sources *.cc)

## Compiler instructions

remove_definitions (-dynamiclib -install_name)

add_library(pydal MODULE
  ${dal_sources}
  ${pydal_sources}
  )

include_directories (
  ${NUM_UTIL_INCLUDES}
  )

## linker instructions

target_link_libraries(pydal
  ${dal_link_libraries}
  ${PYTHON_LIBRARIES}
  ${BOOST_LIBRARIES}
  ${NUM_UTIL_LIBRARIES}
  ${CFITSIO_LIBRARIES}
  ${WCSLIB_LIBRARIES}
  ${LAPACK_LIBRARIES}
  ${HAVE_LIBDL}
  ${HAVE_LIBPTHREAD}
  ${HAVE_LIBUTIL}
  )

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

## Installation instructions

install (
  TARGETS pydal
  LIBRARY DESTINATION lib/python
  )
