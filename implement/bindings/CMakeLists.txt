
  
  ##______________________________________________
  ## Source files with the wrapping code

  file (GLOB pydal_sources *.cc)
  
  ##______________________________________________
  ## Compiler instructions
  
  remove_definitions (-dynamiclib -install_name)

  include_directories (
    ${NUMUTIL_INCLUDES}
    )
  
  add_library(pydal MODULE
    ${dal_sources}
    ${pydal_sources}
    )
  
  ##______________________________________________
  ## linker instructions
  
  target_link_libraries (pydal
    ${dal_link_libraries}
    ${PYTHON_LIBRARIES}
    ${BOOST_LIBRARIES}
    ${NUMUTIL_LIBRARIES}
    ${CFITSIO_LIBRARIES}
    ${WCSLIB_LIBRARIES}
    ${LAPACK_LIBRARIES}
    ${HAVE_LIBPTHREAD}
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
    LIBRARY DESTINATION lib/python
    )
  
