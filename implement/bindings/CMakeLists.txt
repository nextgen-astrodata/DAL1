
  
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

  set (pydal_link_libraries
    ${dal_link_libraries}
    ${PYTHON_LIBRARIES}
    ${BOOST_LIBRARIES}
    ${NUMUTIL_LIBRARIES}
    ${LAPACK_LIBRARIES}
    ${HAVE_LIBPTHREAD}
    )

  if (CFITSIO_LIBRARIES)
    list (APPEND pydal_link_libraries ${CFITSIO_LIBRARIES})
  endif (CFITSIO_LIBRARIES)
  
  if (WCSLIB_LIBRARIES)
    list (APPEND pydal_link_libraries ${WCSLIB_LIBRARIES})
  endif (WCSLIB_LIBRARIES)
  
  target_link_libraries (pydal ${pydal_link_libraries} )
  
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