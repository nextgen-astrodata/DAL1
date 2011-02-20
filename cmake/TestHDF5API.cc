
#include <iostream>
#include <string>
#include <hdf5.h>

int main ()
{
  std::string name  = "UNDEFINED";
  herr_t status     = 0;
  hid_t fileID      = 0;
  hid_t groupID     = 0;
  hid_t attributeID = 0;
  hid_t accessProperties = H5P_DEFAULT;

  /* Generic opening of HDF5 object */
  groupID = H5Gopen (fileID,
		     name.c_str(),
		     accessProperties);
  
  /* Open an attribute atached to the object */
  attributeID = H5Aopen (groupID,
			 name.c_str(),
			 accessProperties);

  status = H5Aclose (attributeID);
  status = H5Gclose (groupID);

  return 1;
}
