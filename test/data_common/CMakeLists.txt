
##_______________________________________________________________________________
##                                                           List of source files

set (tests_data_hl 
  tFilename
  tCommonAttributes
  )

##_______________________________________________________________________________
##                                                     Build/Install instructions

foreach (src ${tests_data_hl})
  ## compiler instructions
  add_executable(${src} ${src}.cc)
  ## linker instructions
  if (HAVE_CASA OR HAVE_CASACORE)
    target_link_libraries(${src}
      ${CASA_LIBRARIES}
      dal
      ${CASA_LIBRARIES}
      ${CFITSIO_LIBRARIES}
      ${dal_link_libraries}
      ${HAVE_MALLOC_DEBUG}
      )
  else (HAVE_CASA OR HAVE_CASACORE)
    target_link_libraries(${src} dal)
  endif (HAVE_CASA OR HAVE_CASACORE)
  ## installation instructions
  install (TARGETS ${src}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  )
endforeach (src)

##_______________________________________________________________________________
##                                                           Testing instructions

add_test (tFilename tFilename)
add_test (tCommonAttributes tCommonAttributes)
