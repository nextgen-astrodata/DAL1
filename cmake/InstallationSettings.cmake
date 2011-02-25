
## ==============================================================================
#
#    .                           DAL_INSTALL_PREFIX       [/opt/dal]
#    ├── bin                     DAL_INSTALL_BINDIR       [DAL_INSTALL_PREFIX/bin]
#    ├── include                 DAL_INSTALL_INCLUDEDIR   [DAL_INSTALL_PREFIX/include]
#    ├── lib                     DAL_INSTALL_LIBDIR       [DAL_INSTALL_PREFIX/lib]
#    └── share                   DAL_INSTALL_DATAROOTDIR  [DAL_INSTALL_PREFIX/share]
#        ├── doc                 DAL_INSTALL_DOCDIR       [DAL_INSTALL_DATAROOTDIR/doc]
#        ├── xml
#        └── macros
#
## ==============================================================================

## Set installation prefix

if (DAL_INSTALL_PREFIX)
  set (CMAKE_INSTALL_PREFIX ${DAL_INSTALL_PREFIX} CACHE PATH
    "CMake installation area."
    FORCE
    )
else (DAL_INSTALL_PREFIX)
  set (DAL_INSTALL_PREFIX "/opt/dal")
  set (CMAKE_INSTALL_PREFIX ${DAL_INSTALL_PREFIX} CACHE PATH
    "CMake installation area."
    FORCE
    )
endif (DAL_INSTALL_PREFIX)

if (DAL_LOFARSOFT_INSTALL)

  ## Installation conforming to setup for LOFAR User Software (LUS)
  set (DAL_INSTALL_BINDIR       "${CMAKE_INSTALL_PREFIX}/bin"          )
  set (DAL_INSTALL_INCLUDEDIR   "${CMAKE_INSTALL_PREFIX}/include/dal"  )
  set (DAL_INSTALL_LIBDIR       "${CMAKE_INSTALL_PREFIX}/lib"          )
  set (DAL_INSTALL_DATAROOTDIR  "${CMAKE_INSTALL_PREFIX}/share"        )
  set (DAL_INSTALL_DOCDIR       "${DAL_INSTALL_DATAROOTDIR}/doc"       )

else (DAL_LOFARSOFT_INSTALL)

  ## Configuration for stand-alone installation

  if (NOT DAL_INSTALL_BINDIR)
    set (DAL_INSTALL_BINDIR "${CMAKE_INSTALL_PREFIX}/bin" )
  endif (NOT DAL_INSTALL_BINDIR)

  if (NOT DAL_INSTALL_INCLUDEDIR)
    set (DAL_INSTALL_INCLUDEDIR "${CMAKE_INSTALL_PREFIX}/include" )
  endif (NOT DAL_INSTALL_INCLUDEDIR)
  
  if (NOT DAL_INSTALL_LIBDIR)
    set (DAL_INSTALL_LIBDIR "${CMAKE_INSTALL_PREFIX}/lib" )
  endif (NOT DAL_INSTALL_LIBDIR)
  
  if (NOT DAL_INSTALL_DATAROOTDIR)
    set (DAL_INSTALL_DATAROOTDIR "${CMAKE_INSTALL_PREFIX}/share" )
  endif (DAL_INSTALL_DATAROOTDIR)
  
  if (NOT DAL_INSTALL_DOCDIR)
    set (DAL_INSTALL_DOCDIR "${DAL_INSTALL_DATAROOTDIR}/doc" )
  endif (DAL_INSTALL_DOCDIR)
  
endif (DAL_LOFARSOFT_INSTALL)

## ==============================================================================

# skip the full RPATH for the build tree
set (CMAKE_SKIP_BUILD_RPATH YES)

# when building, use the install RPATH already
# (so it doesn't need to relink when installing)
set (CMAKE_BUILD_WITH_INSTALL_RPATH YES)

# the RPATH to be used when installing
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH YES)

