
#include <iostream>
#include <hdf5.h>

int main ()
{
  unsigned int versionMajor;
  unsigned int versionMinor;
  unsigned int versionRelease;

  herr_t h5err = H5get_libversion (&versionMajor,
				   &versionMinor,
				   &versionRelease);

  std::cout << "HDF5_Version_Major   = [" << versionMajor   << "]"<< std::endl;
  std::cout << "HDF5_Version_Minor   = [" << versionMinor   << "]"<< std::endl;
  std::cout << "HDF5_Version_Release = [" << versionRelease << "]"<< std::endl;

  return h5err;
}
