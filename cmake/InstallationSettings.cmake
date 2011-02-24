
if (DAL_INSTALL_PREFIX)
  set (CMAKE_INSTALL_PREFIX ${DAL_INSTALL_PREFIX} CACHE PATH
    "CMake installation area."
    FORCE
    )
else (DAL_INSTALL_PREFIX)
  set (CMAKE_INSTALL_PREFIX "/opt/dal" CACHE PATH
    "CMake installation area."
    FORCE
    )
endif (DAL_INSTALL_PREFIX)

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

