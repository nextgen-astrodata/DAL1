
#include "H5Base.h"

// ==============================================================================
//
//  class H5Base
//
// ==============================================================================

//_______________________________________________________________________________
//                                                                        ~H5Base

H5Base::~H5Base ()
{
  if (location_p > 0) {
    herr_t h5error;
    H5I_type_t object_type = H5Iget_type(location_p);
    switch (object_type) {
    case H5I_FILE:
      h5error = H5Fclose (location_p);
      break;
    case H5I_GROUP:
      h5error = H5Gclose (location_p);
      break;
    case H5I_DATATYPE:
      h5error = H5Tclose (location_p);
      break;
    case H5I_DATASPACE:
      h5error = H5Sclose (location_p);
      break;
    case H5I_DATASET:
      h5error = H5Dclose (location_p);
      break;
    case H5I_ATTR:
      h5error = H5Aclose (location_p);
      break;
    default:
      break;
    };
  } else {
    std::cout << "" << std::endl;
  }
}

//_______________________________________________________________________________
//                                                                  setAttributes

bool H5Base::setAttributes (std::set<std::string> const &attr)
{
  bool status (true);
  
  if (attr.size() > 0) {
    attributes_p.clear();
    attributes_p = attr;
  } else {
    status = false;
  }
  
  return status;
}

// ==============================================================================
//
//  class H5Base
//
// ==============================================================================

bool H5File::open (hid_t const &location,
		   std::string const &name,
		   bool const &create)
{
  return false;
}
